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
#include "fontdiff/font_loader.h"
#include "fontdiff/sha1.h"

namespace fontdiff {

FontLoader::FontLoader() {
}

FontLoader::~FontLoader() {
}

Font* FontLoader::Load(const std::string& path) {
  std::string key = GetFileSHA1(path);
  Font* font = fonts_[key];
  if (!font) {
    font = fonts_[key] = Font::Load(path);
  }
  return font;
}

FontCollection* FontLoader::LoadCollection(
    const std::vector<std::string>& paths) {
  std::vector<const Font*> fonts;
  for (int i = 0; i < paths.size(); ++i) {
    fonts.push_back(Load(paths[i]));
  }
  return new FontCollection(fonts);
}

}  // namespace fontdiff
