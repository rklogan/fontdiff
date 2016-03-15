#ifndef TTFDIFF_PARAGRAPH_H_
#define TTFDIFF_PARAGRAPH_H_

#include <string>
#include <vector>

#include <unicode/stringpiece.h>
#include <unicode/ubidi.h>
#include <unicode/unistr.h>

namespace ttfdiff {

class FontCollection;
class Language;
class ShapedText;
class Style;

class Paragraph {
 public:
  Paragraph(const FontCollection* beforeFonts,
	    const FontCollection* afterFonts);
  ~Paragraph();
  void AppendSpan(const icu::StringPiece& text, const Style* style);
  void Layout();

 private:
  struct Span {
    int32_t limit;
    const Style* style;
  };

  void ShapeBidiRun(int32_t start, int32_t limit, UBiDiLevel bidiLevel);
  size_t FindSpan(int32_t start) const;

  const FontCollection* beforeFonts_;
  const FontCollection* afterFonts_;
  icu::UnicodeString text_;
  std::vector<Span> spans_;
  std::vector<ShapedText*> shapedRuns_;
};

}  // namespace ttfdiff

#endif  // TFDIFF_PARAGRAPH_H_
