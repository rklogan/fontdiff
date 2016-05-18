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

#include <ft2build.h>
#include FT_GLYPH_H

#include "fontdiff/font.h"
#include "fontdiff/style_key.h"

namespace fontdiff {

int StyleKey::CompareTo(const StyleKey& other) const {
  int cmp = family.compare(other.family);
  if (cmp != 0) {
    return cmp;
  }

  if (width < other.width) {
    return -1;
  } else if (width > other.width) {
    return 1;
  }

  if (weight < other.weight) {
    return -1;
  } else if (weight > other.weight) {
    return 1;
  }

  if (italicAngle < other.italicAngle) {
    return -1;
  } else if (italicAngle > other.italicAngle) {
    return 1;
  }

  return style.compare(other.style);
}


StyleKey StyleKey::Make(const Font& font) {
  StyleKey result;
  result.family = font.GetFamily();
  result.style = font.GetStyle();
  result.width = font.GetWidth();
  result.weight = font.GetWeight();
  result.italicAngle = font.GetItalicAngle();
  return result;
}

}  // namespace fontdiff
