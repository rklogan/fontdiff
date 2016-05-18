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

#ifndef FONTDIFF_LANGUAGE_H_
#define FONTDIFF_LANGUAGE_H_

#include <hb.h>
#include <unicode/brkiter.h>
#include <unicode/locid.h>

namespace fontdiff {

class Language {
 public:
  Language(const std::string& bcp47);
  ~Language();
  const std::string& GetBCP47Code() const { return bcp47_; }
  icu::BreakIterator* GetLineBreaker() const { return lineBreaker_; }
  icu::BreakIterator* GetWordBreaker() const { return wordBreaker_; }
  hb_language_t GetHarfbuzzLanguage() const { return hb_language_; }
  hb_script_t GetHarfbuzzScript() const { return hb_script_; }

 private:
  std::string bcp47_;
  icu::Locale locale_;
  icu::BreakIterator* lineBreaker_;
  icu::BreakIterator* wordBreaker_;
  hb_language_t hb_language_;
  hb_script_t hb_script_;
};

}  // namespace fontdiff

#endif // FONTDIFF_LANGUAGE_H_
