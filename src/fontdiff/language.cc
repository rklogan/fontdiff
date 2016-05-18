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
