#ifndef TTFDIFF_PARAGRAPH_H_
#define TTFDIFF_PARAGRAPH_H_

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_GLYPH_H

#include <unicode/stringpiece.h>
#include <unicode/ubidi.h>
#include <unicode/unistr.h>

namespace ttfdiff {

class DiffJob;
class FontCollection;
class Language;
class Line;
class ShapedText;
class Style;

class Paragraph {
 public:
  Paragraph(DiffJob* job,
	    const FontCollection* beforeFonts,
	    const FontCollection* afterFonts);
  ~Paragraph();
  void AppendSpan(const icu::StringPiece& text, const Style* style);
  void Layout(FT_F26Dot6 width);

 private:
  struct Span {
    int32_t limit;
    const Style* style;
  };

  void ShapeBidiRun(int32_t start, int32_t limit, UBiDiLevel bidiLevel);
  size_t FindSpan(int32_t start) const;
  size_t FindShapedRun(const std::vector<ShapedText*>& runs,
		       int32_t start) const;
  void ShapeSpan(int32_t start, int32_t limit,
                 UBiDiLevel bidiLevel,
		 const FontCollection* fonts, const Style* style,
		 std::vector<ShapedText*>* runs);
  void FindPotentialLineBreaks(std::vector<int32_t>* breaks);
  void FindPotentialLineBreaks(int32_t start, int32_t limit,
			       const Language* language,
			       std::vector<int32_t>* breaks);

  void MeasureText(bool before, int32_t start, int32_t limit,
		   FT_F26Dot6* xAdvance,
		   FT_F26Dot6* ascender, FT_F26Dot6* descender) const;

  void AddRunsToLine(bool before, int32_t start, int32_t limit, Line* line);

  DiffJob* job_;
  const FontCollection* beforeFonts_;
  const FontCollection* afterFonts_;
  icu::UnicodeString text_;
  std::vector<Span> spans_;
  std::vector<ShapedText*> beforeRuns_, afterRuns_;
  std::vector<Line*> beforeLines_, afterLines_;
};

}  // namespace ttfdiff

#endif  // TFDIFF_PARAGRAPH_H_
