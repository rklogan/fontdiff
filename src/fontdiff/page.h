#ifndef FONTDIFF_PAGE_H_
#define FONTDIFF_PAGE_H_

#include <vector>

#include <ft2build.h>
#include FT_GLYPH_H

typedef struct _cairo cairo_t;

namespace fontdiff {

class Line;

class Page {
 public:
  Page();
  ~Page();
  void AddLine(Line* line, FT_F26Dot6 x, FT_F26Dot6 y);
  void Render(cairo_t* gc) const;
  FT_F26Dot6 GetY() const { return yPos_; }

 private:
  FT_F26Dot6 yPos_;
  struct PositionedLine {
    Line* line;  // owned
    FT_F26Dot6 x, y;
  };
  std::vector<PositionedLine> lines_;
};

}  // namespace fontdiff

#endif  // FONTDIFF_PAGE_H_
