#ifndef FONTDIFF_LINE_DIFFER_H_
#define FONTDIFF_LINE_DIFFER_H_

namespace fontdiff {
class Line;
bool FindDeltas(Line* before, Line* after);
}  // namespace fontdiff

#endif  // FONTDIFF_LINE_DIFFER_H_
