#include "ttfdiff/language.h"
#include "ttfdiff/style.h"

namespace ttfdiff {

Style::Style(const Style* parent, const Language* language,
	     const std::string& spec)
  : language_(language),
    textSize_(parent ? parent->textSize_ : 12.0) {

  // TODO: Properly parse spec string.
  double textSize = strtod(spec.c_str(), NULL);
  if (textSize > 0 && textSize < 1000) {
    textSize_ = textSize;
  }
}

Style::~Style() {
}

}  // namespace ttfdiff
