#ifndef TTFDIFF_LINE_DIFFER_H_
#define TTFDIFF_LINE_DIFFER_H_

namespace ttfdiff {
class Line;
bool FindDeltas(Line* before, Line* after);
}  // namespace ttfdiff

#endif  // TTFDIFF_LINE_DIFFER_H_
