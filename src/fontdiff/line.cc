/*
 * Copyright 2016 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vector>

#include "cairo.h"

#include "fontdiff/cairo_helper.h"
#include "fontdiff/line.h"
#include "fontdiff/shaped_text.h"

namespace fontdiff {

Line::Line(FT_F26Dot6 width)
  : width_(width), xAdvance_(0), ascender_(0), descender_(0),
    backgroundColor_(0xffffff) {
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
  descender_ = std::min(descender_, text->GetDescender());
}

void Line::Render(cairo_t* gc, FT_F26Dot6 x, FT_F26Dot6 y) const {
  for (const Run& run : runs_) {
    run.text->Render(run.start, run.limit, gc, x + run.x, y);
  }
}

void Line::RenderHighlights(cairo_t* gc, FT_F26Dot6 x, FT_F26Dot6 y) const {
  if (backgroundColor_ == 0xffffff) {
    return;
  }
  SetSourceColor(gc, backgroundColor_);
  cairo_rectangle(gc, x / 64.0 - 1.0, (y - ascender_) / 64.0,
		  width_/64.0 + 2.0, (ascender_ - descender_) / 64.0);
  cairo_fill(gc);
  SetSourceColor(gc, 0);
}

}  // namespace fontdiff
