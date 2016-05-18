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

#ifndef FONTDIFF_STYLE_KEY_H_
#define FONTDIFF_STYLE_KEY_H_

#include <ft2build.h>
#include FT_GLYPH_H

#include <map>
#include <string>

namespace fontdiff {

class Font;

struct StyleKey {
  std::string family, style;
  FT_UShort width, weight;
  FT_Fixed italicAngle;

  static StyleKey Make(const Font& font);
  int CompareTo(const StyleKey& other) const;
};


struct StyleKeyLess {
  bool operator()(const StyleKey& a, const StyleKey& b) {
    return a.CompareTo(b) < 0;
  }
};

typedef std::multimap<StyleKey, Font*, StyleKeyLess> StyleFontMap;

}  // namespace fontdiff

#endif  // FONTDIFF_STYLE_KEY_H_
