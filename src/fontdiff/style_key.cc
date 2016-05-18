#include <ft2build.h>
#include FT_GLYPH_H

#include "fontdiff/font.h"
#include "fontdiff/style_key.h"

namespace fontdiff {

int StyleKey::CompareTo(const StyleKey& other) const {
  int cmp = family.compare(other.family);
  if (cmp != 0) {
    return cmp;
  }

  if (width < other.width) {
    return -1;
  } else if (width > other.width) {
    return 1;
  }

  if (weight < other.weight) {
    return -1;
  } else if (weight > other.weight) {
    return 1;
  }

  if (italicAngle < other.italicAngle) {
    return -1;
  } else if (italicAngle > other.italicAngle) {
    return 1;
  }

  return style.compare(other.style);
}


StyleKey StyleKey::Make(const Font& font) {
  StyleKey result;
  result.family = font.GetFamily();
  result.style = font.GetStyle();
  result.width = font.GetWidth();
  result.weight = font.GetWeight();
  result.italicAngle = font.GetItalicAngle();
  return result;
}

}  // namespace fontdiff
