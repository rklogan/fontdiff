#include "ttfdiff/diff_job.h"
#include "ttfdiff/line.h"
#include "ttfdiff/page.h"

namespace ttfdiff {

Page::Page()
  : yPos_(DiffJob::marginWidth) {
}

Page::~Page() {
  for (auto l : lines_) {
    delete l.line;
  }
}

void Page::AddLine(Line* line, FT_F26Dot6 x, FT_F26Dot6 y) {
  if (!line) {
    return;
  }
  PositionedLine pline;
  pline.line = line;
  pline.x = x;
  pline.y = y;
  lines_.push_back(pline);
  yPos_ += line->GetAscender() - line->GetDescender();
}

void Page::Render(cairo_t* gc) const {
  for (const PositionedLine& pline : lines_) {
    pline.line->RenderHighlights(gc, pline.x, pline.y);
  }
  for (const PositionedLine& pline : lines_) {
    pline.line->Render(gc, pline.x, pline.y);
  }
}
  
}  // namespace ttfdiff
