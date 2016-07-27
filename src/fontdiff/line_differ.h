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

#ifndef FONTDIFF_LINE_DIFFER_H_
#define FONTDIFF_LINE_DIFFER_H_

#include <vector>

#include <ft2build.h>
#include FT_GLYPH_H

namespace fontdiff {

class Line;

struct DeltaRange {
  FT_F26Dot6 x;
  FT_F26Dot6 width;
};

bool FindDeltas(const Line* before, const Line* after,
                std::vector<DeltaRange>* removals,
                std::vector<DeltaRange>* additions);

}  // namespace fontdiff

#endif  // FONTDIFF_LINE_DIFFER_H_
