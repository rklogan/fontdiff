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
