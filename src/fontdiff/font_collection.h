#ifndef FONTDIFF_FONT_COLLECTION_H_
#define FONTDIFF_FONT_COLLECTION_H_

#include <vector>
#include <unicode/utypes.h>

namespace fontdiff {

class Font;
class Style;

class FontCollection {
 public:
  FontCollection(std::vector<const Font*> fonts);
  ~FontCollection();
  const Font* FindFont(UChar32 codepoint, const Style* style,
		       const Font* curFont) const;

 private:
  std::vector<const Font*> fonts_;
};

}  // namespace fontdiff

#endif  // FONTDIFF_FONT_COLLECTION_H_
