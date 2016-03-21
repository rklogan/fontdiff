#include <string>
#include <vector>
#include "ttfdiff/language.h"
#include "ttfdiff/style.h"

namespace ttfdiff {

std::vector<std::string> split(const std::string& text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, limit = 0;
  while ((limit = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, limit - start));
    start = limit + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

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
