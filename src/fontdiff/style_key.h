#ifndef FONTDIFF_STYLE_KEY_H_
#define FONTDIFF_STYLE_KEY_H_

#include <ft2build.h>
#include FT_GLYPH_H

#include <map>
#include <string>

namespace fontdiff {

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

}  // namespace fontdiff

#endif  // FONTDIFF_STYLE_KEY_H_
