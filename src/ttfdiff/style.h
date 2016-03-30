#ifndef TTFDIFF_STYLE_H_
#define TTFDIFF_STYLE_H_

#include <string>

namespace ttfdiff {

class Font;
class Language;

class Style {
 public:
  Style(const Style* parent, const Language* language,
        const std::string& spec);
  ~Style();
  const Language* GetLanguage() const { return language_; }
  double GetTextSize() const { return fontSize_; }
  int32_t GetFontScore(const Font& font) const;

 private:
  void SetProperty(const std::string& key, const std::string& value);
  void SetFontSize(double size);
  void SetFontWeight(double weight);

  const Language* language_;
  double fontSize_;
  double fontWeight_;  // 100..900
};
  
}  // namespace ttfdiff

#endif  // TFDIFF_STYLE_H_
