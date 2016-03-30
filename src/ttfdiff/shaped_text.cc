#include <string>
#include <vector>

#include <cairo/cairo.h>
#include <hb.h>
#include <hb-ft.h>
#include <hb-icu.h>
#include <unicode/unistr.h>

#include "ttfdiff/font.h"
#include "ttfdiff/icu_helper.h"
#include "ttfdiff/language.h"
#include "ttfdiff/shaped_text.h"
#include "ttfdiff/style.h"

namespace ttfdiff {

ShapedText::ShapedText(const UChar* text, int32_t start, int32_t limit,
                       UBiDiLevel bidiLevel,
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
  props.script = lang ? lang->GetHarfbuzzScript() : HB_SCRIPT_INVALID;
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
  FT_Face face = font_->GetFreetypeFace();
  FT_F26Dot6 textSize = static_cast<FT_F26Dot6>(style_->GetTextSize() * 64);
  FT_Set_Char_Size(face, textSize, textSize, 72, 72);
  // TODO: Take ascender and descender from the OpenType BASE table if present.
  ascender_ = face->size->metrics.ascender;
  descender_ = face->size->metrics.descender;
  hb_font_t* hbFont = hb_ft_font_create(face, NULL);
  // TODO: hb_ft_font_set_load_flags(hbFont, FT_LOAD_NO_HINTING);
  hb_shape(hbFont, hb_buffer_, NULL, 0);
  hb_font_destroy(hbFont);
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
      cairoGlyph.y = (y + pos[gi].y_offset) / 64.0;
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
      cairoGlyph.y = (y + pos[gi].y_offset) / 64.0;
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
  cairo_set_font_size(gc, style_->GetTextSize());
  if (cairoClusters.size() > 0) {
    cairo_show_text_glyphs(gc, utf8.c_str(), utf8.size(),
			   &cairoGlyphs.front(), cairoGlyphs.size(),
			   &cairoClusters.front(), cairoClusters.size(),
			   cairoFlags);
  } else {
    cairo_show_glyphs(gc, &cairoGlyphs.front(), cairoGlyphs.size());
  }
  if (false) {
    printf("ShapedText::Render %d..%d; x: %g, y: %g; utf8:\"%s\" "
	   "size: %g font: %s\n",
	   start, limit, x/64.0, y/64.0, utf8.c_str(), style_->GetTextSize(),
	   font_->GetPostScriptName().c_str());
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

}  // namespace ttfdiff
