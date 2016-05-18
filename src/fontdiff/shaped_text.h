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

#ifndef FONTDIFF_SHAPED_TEXT_H_
#define FONTDIFF_SHAPED_TEXT_H_

#include <string>

#include <ft2build.h>
#include FT_GLYPH_H
#include <hb.h>
#include <unicode/ubidi.h>

typedef struct _cairo cairo_t;

namespace fontdiff {

class Font;
class Style;

class ShapedText {
 public:
  ShapedText(const UChar* text, int32_t start, int32_t limit,
             UBiDiLevel bidiLevel, const Font* font, const Style* style);
  ~ShapedText();
  int32_t GetStart() const { return start_; }
  int32_t GetLimit() const { return limit_; }
  FT_F26Dot6 GetAscender() const { return ascender_; }
  FT_F26Dot6 GetDescender() const { return descender_; }
  FT_F26Dot6 GetXAdvance(int32_t start, int32_t limit) const;
  void Render(int32_t start, int32_t limit,
	      cairo_t* gc, FT_F26Dot6 x, FT_F26Dot6 y) const;
  bool IsCovering(int32_t start, int32_t limit) const;

 private:
  void Shape();
  static int32_t AppendUTF8(const UChar* text,
			    int32_t start, int32_t limit,
			    std::string* out);
  static unsigned int FindGlyph(int32_t cluster, hb_glyph_info_t* glyphs,
				unsigned int numGlyphs, bool backward);

  const UChar* text_;
  int32_t start_, limit_;
  UBiDiLevel bidiLevel_;
  hb_buffer_t* hb_buffer_;
  const Font* font_;
  const Style* style_;
  FT_F26Dot6 ascender_, descender_;
};

}  // namespace fontdiff

#endif  // FONTDIFF_SHAPED_TEXT_H_
