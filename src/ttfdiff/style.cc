#include <string>
#include <vector>
#include "ttfdiff/font.h"
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
    textSize_(parent ? parent->textSize_ : 12.0),
    weight_(parent ? parent->weight_ : 400) {
  // TODO: Properly parse spec string.
  double textSize = strtod(spec.c_str(), NULL);
  if (textSize > 0 && textSize < 1000) {
    textSize_ = textSize;
  }
}

Style::~Style() {
}

int32_t Style::GetFontScore(const Font& font) const {
  // TODO: Implement properly, based on style specification.
  int32_t delta = 0;
  delta += std::abs(font.GetWeight() - weight_);
  if (font.GetItalicAngle() != 0) {
    delta += 50;
  }
  return delta;
}
  
}  // namespace ttfdiff
