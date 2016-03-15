#ifndef TTFDIFF_STYLE_KEY_H_
#define TTFDIFF_STYLE_KEY_H_

#include <ft2build.h>
#include FT_GLYPH_H

#include <map>
#include <string>

namespace ttfdiff {

class Font;

struct StyleKey {
  std::string family, style;
  FT_UShort width, weight;
  FT_Fixed italicAngle;

  static StyleKey Make(const Font& font);
  int CompareTo(const StyleKey& other) const;
};


struct StyleKeyLess {
  bool operator()(const StyleKey& a, const StyleKey& b) {
    return a.CompareTo(b) < 0;
  }
};

typedef std::multimap<StyleKey, Font*, StyleKeyLess> StyleFontMap;

}  // namespace ttfdiff

#endif  // TTFDIFF_STYLE_KEY_H_
