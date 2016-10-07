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

#include <cstdint>
#include <map>
#include <string>

namespace fontdiff {

class Font;
class Language;

typedef std::map<std::string, uint32_t> FontFeatures;

class Style {
 public:
  Style(const Style* parent, const Language* language,
        const std::string& spec);
  ~Style();
  const Language* GetLanguage() const { return language_; }
  double GetFontSize() const { return fontSize_; }
  double GetFontWeight() const { return fontWeight_; }
  double GetFontWidth() const { return fontWidth_; }
  double GetFontScore(const Font& font) const;
  const FontFeatures& GetFontFeatures() const { return fontFeatures_; }

 private:
  void SetProperty(const std::string& key, const std::string& value);
  void SetFontSize(double size);
  void SetFontWeight(double weight);
  void SetFontWidth(double width);
  void SetFontFeature(const std::string& feature, uint32_t value);
  void SetFontFeatureSetting(const std::string& setting);

  const Language* language_;
  double fontSize_;
  double fontWeight_;  // 100.0 .. 1000.0, default 400.0
  double fontWidth_;   // 50.0 .. 200.0, default 100.0
  FontFeatures fontFeatures_;
};

}  // namespace fontdiff

#endif  // TFDIFF_STYLE_H_
