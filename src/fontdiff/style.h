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

#ifndef FONTDIFF_STYLE_H_
#define FONTDIFF_STYLE_H_

#include <string>

namespace fontdiff {

class Font;
class Language;

class Style {
 public:
  Style(const Style* parent, const Language* language,
        const std::string& spec);
  ~Style();
  const Language* GetLanguage() const { return language_; }
  double GetFontSize() const { return fontSize_; }
  double GetFontWeight() const { return fontWeight_; }
  int32_t GetFontScore(const Font& font) const;

 private:
  void SetProperty(const std::string& key, const std::string& value);
  void SetFontSize(double size);
  void SetFontWeight(double weight);

  const Language* language_;
  double fontSize_;
  double fontWeight_;  // 100..900
};

}  // namespace fontdiff

#endif  // TFDIFF_STYLE_H_
