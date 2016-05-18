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
