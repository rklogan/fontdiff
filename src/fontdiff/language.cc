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

#include <hb-icu.h>
#include <unicode/brkiter.h>
#include <unicode/locid.h>
#include <unicode/uscript.h>

#include "fontdiff/icu_helper.h"
#include "fontdiff/language.h"

namespace fontdiff {

Language::Language(const std::string& bcp47)
  : bcp47_(bcp47),
    lineBreaker_(NULL), wordBreaker_(NULL),
    hb_language_(  // global pool, owned by harfbuzz
        hb_language_from_string(bcp47_.c_str(), bcp47_.size())),
    hb_script_(HB_SCRIPT_UNKNOWN) {
  UErrorCode error = U_ZERO_ERROR;
  char localeName[300], maximizedName[300], scriptName[300];
  uloc_forLanguageTag(bcp47.c_str(), localeName, sizeof(localeName),
		      NULL, &error);
  CheckUErrorCode(error);
  locale_ = icu::Locale::createCanonical(localeName);
  uloc_addLikelySubtags(localeName, maximizedName, sizeof(maximizedName),
                        &error);
  CheckUErrorCode(error);
  uloc_getScript(maximizedName, scriptName, sizeof(scriptName), &error);
  if (U_SUCCESS(error)) {
    UScriptCode scriptCode = static_cast<UScriptCode>(
        u_getPropertyValueEnum(UCHAR_SCRIPT, scriptName));
    hb_script_ = hb_icu_script_to_script(scriptCode);
  } else {
    hb_script_ = HB_SCRIPT_UNKNOWN;
    error = U_ZERO_ERROR;
  }

  lineBreaker_ = icu::BreakIterator::createLineInstance(locale_, error);
  CheckUErrorCode(error);
  wordBreaker_ = icu::BreakIterator::createWordInstance(locale_, error);
  CheckUErrorCode(error);
}

Language::~Language() {
  delete lineBreaker_;
  delete wordBreaker_;
}

}  // namespace fontdiff
