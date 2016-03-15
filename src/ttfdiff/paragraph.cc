#include <string>
#include <unicode/ubidi.h>
#include <unicode/unistr.h>

#include "ttfdiff/icu_helper.h"
#include "ttfdiff/paragraph.h"
#include "ttfdiff/style.h"

namespace ttfdiff {

Paragraph::Paragraph() {
}
  
Paragraph::~Paragraph() {
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
    std::string s;
    text_.tempSubStringBetween(spanStart, spanLimit).toUTF8String(s);
    printf("  ShapeBidiSpan %d..%d bidiLevel: %d text: \"%s\"\n",
           spanStart, spanLimit, bidiLevel, s.c_str());
    spanStart = spans_[spanIndex].limit;
    ++spanIndex;
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
  //return lo < spans_.size() ? &spans_[lo] : NULL;
  return lo;
}

}  // namespace ttfdiff
