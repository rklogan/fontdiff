#include <string>
#include <unicode/ubidi.h>
#include <unicode/unistr.h>

#include "ttfdiff/font_collection.h"
#include "ttfdiff/icu_helper.h"
#include "ttfdiff/language.h"
#include "ttfdiff/paragraph.h"
#include "ttfdiff/shaped_text.h"
#include "ttfdiff/style.h"

namespace ttfdiff {

Paragraph::Paragraph(const FontCollection* beforeFonts,
		     const FontCollection* afterFonts)
  : beforeFonts_(beforeFonts),
    afterFonts_(afterFonts) {
}

Paragraph::~Paragraph() {
  for (auto s : beforeRuns_) {
    delete s;
  }
  for (auto s : afterRuns_) {
    delete s;
  }
}

void Paragraph::AppendSpan(const icu::StringPiece& text,
                           const Style* style) {
  text_.append(icu::UnicodeString::fromUTF8(text));
  Paragraph::Span span;
  span.limit = text_.length();
  span.style = style;
  spans_.push_back(span);
}

void Paragraph::Layout() {
  UErrorCode err = U_ZERO_ERROR;
  UBiDi* paraBidi = ubidi_openSized(text_.length(), 0, &err);
  CheckUErrorCode(err);
  UBiDi* lineBidi = ubidi_openSized(text_.length(), 0, &err);
  CheckUErrorCode(err);
  ubidi_setPara(paraBidi, text_.getBuffer(), text_.length(),
	        UBIDI_DEFAULT_LTR, NULL, &err);
  CheckUErrorCode(err);
  int32_t start = 0, limit = 1;
  UBiDiLevel bidiLevel = 0;
  while (true) {
    ubidi_getLogicalRun(paraBidi, start, &limit, &bidiLevel);
    if (limit <= start) {
      break;
    }
    ShapeBidiRun(start, limit, bidiLevel);
    start = limit;
  }

  std::vector<int32_t> potentialLineBreaks;
  FindPotentialLineBreaks(&potentialLineBreaks);
  start = 0;
  for (int32_t b : potentialLineBreaks) {
    printf("MaybeBreak %d..%d\n", start, b);
    start = b;
  }

  if (!beforeRuns_.empty() && !afterRuns_.empty()) {
    printf("Runs before: %d after: %d\n",
	   beforeRuns_.size(), afterRuns_.size());
  }
  ubidi_close(lineBidi);
  ubidi_close(paraBidi);  
}

void Paragraph::ShapeBidiRun(
      int32_t start, int32_t limit, UBiDiLevel bidiLevel) {
  limit = std::min(limit, text_.length());
  if (start >= limit) {
    return;
  }
  const size_t spanIndexLimit =
      std::min(spans_.size(), FindSpan(limit - 1) + 1);
  size_t spanIndex = start > 0 ? FindSpan(start) : 0;
  int32_t spanStart = 
    std::max(spanIndex > 0 ? spans_[spanIndex - 1].limit : 0, start);
  while (spanIndex < spanIndexLimit) {
    const Span& span = spans_[spanIndex];
    const int32_t spanLimit = std::min(limit, span.limit);
    ShapeSpan(spanStart, spanLimit, bidiLevel,
	      beforeFonts_, span.style, &beforeRuns_);
    ShapeSpan(spanStart, spanLimit, bidiLevel,
	      afterFonts_, span.style, &afterRuns_);
    spanStart = spans_[spanIndex].limit;
    ++spanIndex;
  }
}

void Paragraph::ShapeSpan(int32_t start, int32_t limit,
			  UBiDiLevel bidiLevel,
			  const FontCollection* fonts, const Style* style,
			  std::vector<ShapedText*>* result) {
  if (start >= limit || !fonts || !style || !result) {
    return;
  }

  int32_t pos = start, last = start;
  const Font* lastFont = NULL;
  while (pos < limit) {
    const UChar32 curChar = text_.char32At(pos);
    const Font* curFont = fonts->FindFont(curChar, style, lastFont);
    if (curFont != lastFont) {
      if (lastFont && curFont && last < pos) {
	result->push_back(
	    new ShapedText(text_.getBuffer(), last, pos,
			   bidiLevel, curFont, style));
      }
      lastFont = curFont;
      last = pos;
    }
    ++pos;
    if (curChar > 0xFFFF) {
      ++pos;
    }
  }
  if (lastFont && last < limit) {
    result->push_back(
        new ShapedText(text_.getBuffer(), last, limit,
		       bidiLevel, lastFont, style));
  }
}

size_t Paragraph::FindSpan(int32_t pos) const {
  size_t lo = 0, hi = spans_.size();
  while (lo < hi) {
    size_t mid = (lo + hi) / 2;
    if (pos < spans_[mid].limit) {
      hi = mid;
    } else {
      lo = mid + 1;
    }
  }
  return lo;
}

void Paragraph::FindPotentialLineBreaks(std::vector<int32_t>* breaks) {
  const Language* curLang = NULL;
  int32_t runStart = 0, curLangStart = 0;
  const size_t riLimit = spans_.size();
  for (size_t ri = 0; ri < riLimit; ++ri) {
    const Span& span = spans_[ri];
    const Language* spanLang = span.style->GetLanguage();
    if (spanLang != curLang) {
      FindPotentialLineBreaks(curLangStart, runStart, curLang, breaks);
      curLang = spanLang;
      curLangStart = runStart;
    }
    runStart = span.limit;
  }
  FindPotentialLineBreaks(curLangStart, text_.length(), curLang, breaks);
}

void Paragraph::FindPotentialLineBreaks(int32_t start, int32_t limit,
					const Language* language,
					std::vector<int32_t>* breaks) {
  if (start >= limit || !language || !breaks) {
    return;
  }

  icu::BreakIterator* breaker = language->GetLineBreaker();
  if (breaker) {
    breaker->setText(text_);
    int32_t cur = breaker->following(start > 0 ? start - 1 : 0);
    while (cur != BreakIterator::DONE && cur < limit) {
      breaks->push_back(cur);
      cur = breaker->next();
    }
  }
}

}  // namespace ttfdiff
