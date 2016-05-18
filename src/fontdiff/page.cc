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

#include "fontdiff/diff_job.h"
#include "fontdiff/line.h"
#include "fontdiff/page.h"

namespace fontdiff {

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
  
}  // namespace fontdiff
