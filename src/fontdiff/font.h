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

#ifndef FONTDIFF_FONT_H_
#define FONTDIFF_FONT_H_

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_GLYPH_H

typedef struct _cairo_font_face cairo_font_face_t;

namespace fontdiff {

class Font {
 public:
  static std::vector<Font*>* Load(const std::string& path);
  ~Font();

  const std::string& GetPostScriptName() const { return psname_; }
  const std::string& GetFamily() const { return family_; }
  const std::string& GetStyle() const { return style_; }
  FT_UShort GetWidth() const { return width_; }
  FT_UShort GetWeight() const { return weight_; }
  FT_Fixed GetItalicAngle() const { return italicAngle_; }
  bool IsCovering(uint32_t codepoint) const;
  FT_Face GetFreetypeFace() const { return ft_face_; }
  cairo_font_face_t* GetCairoFace() const { return cairo_face_; }

 private:
  static FT_UShort MapWeightClass(FT_UShort weight);
  Font(FT_Face face);

  FT_Face ft_face_;
  cairo_font_face_t* cairo_face_;
  std::string psname_;
  std::string family_;
  std::string style_;
  FT_UShort width_, weight_;
  FT_Fixed italicAngle_;
};
    
}  // namespace fontdiff

#endif  // FONTDIFF_FONT_H_
