#ifndef TTFDIFF_FONT_COLLECTION_H_
#define TTFDIFF_FONT_COLLECTION_H_

#include <vector>
#include <unicode/utypes.h>

namespace ttfdiff {

class Font;
class Style;

class FontCollection {
 public:
  FontCollection(const std::vector<const Font*>& fonts);
  ~FontCollection();
  const Font* FindFont(UChar32 codepoint, const Style* style,
		       const Font* curFont) const;

 private:
  std::vector<const Font*> fonts_;
};

}  // namespace ttfdiff

#endif  // TTFDIFF_FONT_COLLECTION_H_
