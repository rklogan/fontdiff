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

#include <stdlib.h>
#include "fontdiff/font.h"
#include "fontdiff/font_collection.h"
#include "fontdiff/font_loader.h"
#include "fontdiff/sha1.h"

namespace fontdiff {

FontLoader::FontLoader() {
}

FontLoader::~FontLoader() {
  for (auto p : fonts_) {
    for (Font* font : *p.second) {
      delete font;
    }
    delete p.second;
  }
}

std::vector<Font*>* FontLoader::Load(const std::string& path) {
  std::string key = GetFileSHA1(path);
  std::vector<Font*>* fonts = fonts_[key];
  if (!fonts) {
    fonts = fonts_[key] = Font::Load(path);
  }
  return fonts;
}

FontCollection* FontLoader::LoadCollection(
    const std::vector<std::string>& paths) {
  std::vector<const Font*> result;
  for (int i = 0; i < paths.size(); ++i) {
    std::vector<Font*>* fonts = Load(paths[i]);
    if (!fonts || fonts->size() == 0) {
      fprintf(stderr, "could not load any fonts from %s\n", paths[i].c_str());
      exit(2);
    }
    for (const Font* font : *fonts) {
      result.push_back(font);
    }
  }
  return new FontCollection(result);
}

}  // namespace fontdiff
