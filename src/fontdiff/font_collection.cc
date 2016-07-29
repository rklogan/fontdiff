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

#include "fontdiff/font.h"
#include "fontdiff/font_collection.h"
#include "fontdiff/style.h"

namespace fontdiff {

FontCollection::FontCollection(std::vector<const Font*> fonts)
  : fonts_(fonts) {
}

FontCollection::~FontCollection() {
}

const Font* FontCollection::FindFont(UChar32 codepoint, const Style* style,
				     const Font* curFont) const {
  if (curFont && curFont->IsCovering(codepoint)) {
    return curFont;
  }

  const Font* bestFont = NULL;
  int32_t bestScore = 0x7fffffff;
  for (const Font* font : fonts_) {
    if (font->IsCovering(codepoint)) {
      const int32_t score = style->GetFontScore(*font);
      if (score < bestScore) {
	bestScore = score;
	bestFont = font;
      }
    }
  }

  return bestFont;
}

}  // namespace fontdiff
