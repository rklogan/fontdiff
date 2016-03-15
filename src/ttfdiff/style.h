#ifndef TTFDIFF_STYLE_H_
#define TTFDIFF_STYLE_H_

#include <string>

namespace ttfdiff {

class Language;

class Style {
 public:
  Style(const Style* parent, const Language* language,
	const std::string& spec);
  ~Style();
  double GetTextSize() const { return textSize_; }

 private:
  const Language* language_;
  double textSize_;
};
  
}  // namespace ttfdiff

#endif  // TFDIFF_STYLE_H_
