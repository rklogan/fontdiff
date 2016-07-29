/*
 * Copyright 2016 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <vector>

#include <cairo.h>
#include <hb.h>
#include <hb-ft.h>
#include <hb-icu.h>
#include <unicode/unistr.h>

#include "fontdiff/font.h"
#include "fontdiff/icu_helper.h"
#include "fontdiff/language.h"
#include "fontdiff/shaped_text.h"
#include "fontdiff/style.h"

namespace fontdiff {

ShapedText::ShapedText(const UChar* text, int32_t start, int32_t limit,
                       UBiDiLevel bidiLevel, hb_script_t script,
                       const Font* font, const Style* style)
  : text_(text), start_(start), limit_(limit), bidiLevel_(bidiLevel),
    font_(font), style_(style),
    hb_buffer_(hb_buffer_create()), ascender_(0), descender_(0) {
  const int32_t length = limit - start;
  hb_buffer_set_unicode_funcs(hb_buffer_, hb_icu_get_unicode_funcs());
  hb_buffer_add_utf16(hb_buffer_, text + start, length, 0, length);
  hb_buffer_set_cluster_level(hb_buffer_,
                              HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
  hb_segment_properties_t props;
  bzero(&props, sizeof(props));
  props.direction = bidiLevel & 1 ? HB_DIRECTION_RTL : HB_DIRECTION_LTR;
  const Language* lang = style->GetLanguage();
  props.language = lang ? lang->GetHarfbuzzLanguage() : HB_LANGUAGE_INVALID;
  props.script = script;
  hb_buffer_set_segment_properties(hb_buffer_, &props);
  Shape();
}

ShapedText::~ShapedText() {
  hb_buffer_destroy(hb_buffer_);
}

void ShapedText::Shape() {
  if (!font_ || !style_) {
    return;
  }

  const double size = style_->GetFontSize();
  FT_Face face = font_->GetFreeTypeFace();
  FT_F26Dot6 textSize = static_cast<FT_F26Dot6>(size * 64);
  FT_Set_Char_Size(face, textSize, textSize, 72, 72);

  hb_font_t* hbFont = font_->GetHarfBuzzFont();
  hb_font_set_scale(hbFont, size * 64, size * 64);

  hb_font_extents_t extents;
  hb_font_get_extents_for_direction(
      hbFont, hb_buffer_get_direction(hb_buffer_), &extents);
  ascender_ = extents.ascender;
  descender_ = extents.descender;

  hb_shape(hbFont, hb_buffer_, NULL, 0);
}

FT_F26Dot6 ShapedText::GetXAdvance(int32_t start, int32_t limit) const {
  start = std::max(start, start_);
  limit = std::min(limit, limit_);
  if (start >= limit) {
    return 0;
  }

  FT_F26Dot6 result = 0;
  unsigned int numGlyphs = hb_buffer_get_length(hb_buffer_);
  hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(hb_buffer_, NULL);
  hb_glyph_position_t* pos = hb_buffer_get_glyph_positions(hb_buffer_, NULL);
  // TODO: Find the first glyph with a binary search instead of linear scan.
  for (unsigned int i = 0; i < numGlyphs; ++i) {
    int32_t curPos = static_cast<int32_t>(glyphInfo[i].cluster) + start_;
    if (curPos >= start && curPos < limit) {
      result += pos[i].x_advance;
    }
    if (curPos >= limit) {
      break;
    }
  }
  return result;
}

bool ShapedText::IsCovering(int32_t start, int32_t limit) const {
  return start < limit_ && limit >= start_;
}

int32_t ShapedText::AppendUTF8(const UChar* text,
                               int32_t start, int32_t limit,
                               std::string* out) {
  int32_t size = 0;
  UErrorCode error = U_ZERO_ERROR;
  UChar kReplacementChar = 0xfffd;
  u_strToUTF8WithSub(NULL, 0, &size, text + start, limit - start,
		     kReplacementChar, NULL, &error);
  if (size > 0) {
    error = U_ZERO_ERROR;
    size_t oldSize = out->size();
    out->resize(oldSize + size);
    char* dest = &out->at(oldSize);
    u_strToUTF8WithSub(dest, size, &size, text + start, limit - start,
                       kReplacementChar, NULL, &error);
    CheckUErrorCode(error);
  }

  return size;
}

void ShapedText::Render(int32_t start, int32_t limit, cairo_t* gc,
			FT_F26Dot6 x, FT_F26Dot6 y) const {
  start = std::max(start, start_);
  limit = std::min(limit, limit_);
  if (start >= limit) {
    return;
  }

  unsigned int numGlyphs = hb_buffer_get_length(hb_buffer_);
  hb_glyph_info_t* glyphs = hb_buffer_get_glyph_infos(hb_buffer_, NULL);
  hb_glyph_position_t* pos = hb_buffer_get_glyph_positions(hb_buffer_, NULL);
  if (numGlyphs == 0 || !glyphs || !pos || !gc) {
    return;
  }
  const bool backward =
      HB_DIRECTION_IS_BACKWARD(hb_buffer_get_direction(hb_buffer_));
  const int glyphStart = FindGlyph(start - start_, glyphs, numGlyphs, backward);
  const int glyphLimit = FindGlyph(limit - start_, glyphs, numGlyphs, backward);
  std::string utf8;
  std::vector<cairo_glyph_t> cairoGlyphs;
  std::vector<cairo_text_cluster_t> cairoClusters;
  cairo_text_cluster_flags_t cairoFlags =
      static_cast<cairo_text_cluster_flags_t>(0);
  if (backward) {
    int gi = glyphStart - 1;
    for (int gi = glyphLimit; gi < glyphStart; ++gi) {
      cairo_glyph_t cairoGlyph;
      cairoGlyph.index = glyphs[gi].codepoint;
      cairoGlyph.x = (x + pos[gi].x_offset) / 64.0;
      cairoGlyph.y = (y - pos[gi].y_offset) / 64.0;
      x += pos[gi].x_advance;
      y += pos[gi].y_advance;
      cairoGlyphs.push_back(cairoGlyph);
    }
    cairoFlags = CAIRO_TEXT_CLUSTER_FLAG_BACKWARD;
  } else {
    unsigned int curClusterStart = glyphStart;
    unsigned int curCluster = glyphs[curClusterStart].cluster;
    unsigned int gi = glyphStart;
    for (; (gi < numGlyphs) &&
           (static_cast<int32_t>(glyphs[gi].cluster) < limit - start_);
	 ++gi) {
      cairo_glyph_t cairoGlyph;
      cairoGlyph.index = glyphs[gi].codepoint;
      cairoGlyph.x = (x + pos[gi].x_offset) / 64.0;
      cairoGlyph.y = (y - pos[gi].y_offset) / 64.0;
      x += pos[gi].x_advance;
      y += pos[gi].y_advance;
      cairoGlyphs.push_back(cairoGlyph);
      if (glyphs[gi].cluster != curCluster) {
	cairo_text_cluster_t cairoCluster;
	cairoCluster.num_bytes = AppendUTF8(text_, curCluster + start_,
					    glyphs[gi].cluster + start_,
					    &utf8);
	cairoCluster.num_glyphs = gi - curClusterStart;
        cairoClusters.push_back(cairoCluster);
        curCluster = glyphs[gi].cluster;
        curClusterStart = gi;
      }
    }
    cairo_text_cluster_t finalCluster;
    finalCluster.num_bytes =
        AppendUTF8(text_, curCluster + start_, limit, &utf8);
    finalCluster.num_glyphs = gi - curClusterStart;
    cairoClusters.push_back(finalCluster);
  }

  cairo_set_font_face(gc, font_->GetCairoFace());
  cairo_set_font_size(gc, style_->GetFontSize());
  if (cairoClusters.size() > 0) {
    cairo_show_text_glyphs(gc, utf8.c_str(), utf8.size(),
			   &cairoGlyphs.front(), cairoGlyphs.size(),
			   &cairoClusters.front(), cairoClusters.size(),
			   cairoFlags);
  } else {
    cairo_show_glyphs(gc, &cairoGlyphs.front(), cairoGlyphs.size());
  }
}

unsigned int ShapedText::FindGlyph(int32_t cluster, hb_glyph_info_t* glyphs,
				   unsigned int numGlyphs, bool backward) {
  unsigned int lo = 0, hi = numGlyphs;
  if (backward) {
    while (lo < hi) {
      unsigned int mid = (lo + hi) / 2;
      if (cluster > static_cast<int32_t>(glyphs[mid].cluster)) {
	hi = mid;
      } else {
	lo = mid + 1;
      }
    }
  } else {
    while (lo < hi) {
      unsigned int mid = (lo + hi) / 2;
      if (cluster <= static_cast<int32_t>(glyphs[mid].cluster)) {
	hi = mid;
      } else {
	lo = mid + 1;
      }
    }
  }
  return lo;
}

}  // namespace fontdiff
