#ifndef TTFDIFF_LANGUAGE_H_
#define TTFDIFF_LANGUAGE_H_

#include <hb.h>
#include <unicode/brkiter.h>
#include <unicode/locid.h>

namespace ttfdiff {

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

}  // namespace ttfdiff

#endif // TTFDIFF_LANGUAGE_H_
