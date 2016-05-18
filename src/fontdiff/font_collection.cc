#include "ttfdiff/font.h"
#include "ttfdiff/font_collection.h"
#include "ttfdiff/style.h"

namespace ttfdiff {

FontCollection::FontCollection(std::vector<const Font*> fonts)
  : fonts_(fonts) {
}

FontCollection::~FontCollection() {
}

const Font* FontCollection::FindFont(UChar32 codepoint, const Style* style,
				     const Font* curFont) const {
  if (curFont && curFont->IsCovering(codepoint)) {
    return curFont;
  }

  const Font* bestFont = NULL;
  int32_t bestScore = 0x7fffffff;
  for (const Font* font : fonts_) {
    if (font->IsCovering(codepoint)) {
      const int32_t score = style->GetFontScore(*font);
      if (score < bestScore) {
	bestScore = score;
	bestFont = font;
      }
    }
  }
  if (false) {
    printf("font change; codepoint: U+%04X curFont: %s result: %s\n",
	   codepoint,
	   curFont ? curFont->GetPostScriptName().c_str() : "null",
	   bestFont ? bestFont->GetPostScriptName().c_str() : "null");
  }
  return bestFont;
}

}  // namespace ttfdiff

