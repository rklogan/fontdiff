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

#include <memory>
#include <string>

#include "cairo.h"
#include "fontdiff/line.h"

namespace fontdiff {

bool FindDeltas(const Line* before, const Line* after) {
  const int scale = 4;
  FT_F26Dot6 width = std::max(before->GetWidth(), after->GetWidth());
  FT_F26Dot6 height = std::max(before->GetHeight(), after->GetHeight());
  cairo_surface_t* beforeSurface =
      cairo_image_surface_create(CAIRO_FORMAT_A1,
				 (width * scale) / 64, (height * scale) / 64);
  cairo_surface_t* afterSurface =
      cairo_image_surface_create(CAIRO_FORMAT_A1,
				 (width * scale) / 64, (height * scale) / 64);

  cairo_t* beforeGC = cairo_create(beforeSurface);
  cairo_t* afterGC = cairo_create(afterSurface);
  cairo_scale(beforeGC, scale, scale);
  cairo_scale(afterGC, scale, scale);
  before->Render(beforeGC, 0, 0);
  after->Render(afterGC, 0, 0);
  cairo_destroy(beforeGC);
  cairo_destroy(afterGC);

  cairo_surface_flush(beforeSurface);
  cairo_surface_flush(afterSurface);
  unsigned char* beforeData = cairo_image_surface_get_data(beforeSurface);
  unsigned char* afterData = cairo_image_surface_get_data(afterSurface);
  const size_t imageHeight =
      static_cast<size_t>(cairo_image_surface_get_height(beforeSurface));
  const size_t imageStride =
      static_cast<size_t>(cairo_image_surface_get_stride(beforeSurface));
  const bool equal =
      memcmp(beforeData, afterData, imageStride * imageHeight) == 0;
  cairo_surface_destroy(beforeSurface);
  cairo_surface_destroy(afterSurface);
  return !equal;  // if the two lines are not equal, we found some deltas
}

}  // namespace fontdiff
