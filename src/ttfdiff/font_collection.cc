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

  for (const Font* font : fonts_) {
    if (font->IsCovering(codepoint)) {
      return font;
    }
    //printf("FindFont: Considering %s\n", font->GetPostScriptName().c_str());
  }

  return NULL;
}

}  // namespace ttfdiff

