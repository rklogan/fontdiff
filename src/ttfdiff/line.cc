#include <vector>
#include "ttfdiff/line.h"
#include "ttfdiff/shaped_text.h"

namespace ttfdiff {

Line::Line()
  : xAdvance_(0), ascender_(0), descender_(0) {
}

Line::~Line() {
}

void Line::AddShapedText(const ShapedText* text,
			 int32_t start, int32_t limit) {
  Run run;
  run.text = text;
  run.x = xAdvance_;
  run.start = std::max(start, text->GetStart());
  run.limit = std::min(limit, text->GetLimit());
  runs_.push_back(run);
  xAdvance_ += text->GetXAdvance(start, limit);
  ascender_ = std::max(ascender_, text->GetAscender());
  descender_ = std::max(descender_, text->GetDescender());
}

void Line::Render(cairo_t* gc, FT_F26Dot6 x, FT_F26Dot6 y) const {
  for (const Run& run : runs_) {
    run.text->Render(run.start, run.limit, gc, x + run.x, y);
  }
}

}  // namespace ttfdiff
