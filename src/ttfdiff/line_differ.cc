#include <memory>
#include <string>

#include "cairo.h"
#include "ttfdiff/line.h"

namespace ttfdiff {

bool FindDeltas(Line* before, Line* after) {
  const int scale = 4;
  FT_F26Dot6 ascender = std::max(before->GetAscender(), after->GetAscender());
  FT_F26Dot6 descender =
      std::min(before->GetDescender(), after->GetDescender());
  FT_F26Dot6 width = std::max(before->GetWidth(), after->GetWidth());
  FT_F26Dot6 height = ascender - descender;
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
  before->Render(beforeGC, 0, ascender);
  after->Render(afterGC, 0, ascender);
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

  if (equal) {
    return false;  // no deltas found
  } else {
    before->SetBackgroundColor(0xffecec);
    after->SetBackgroundColor(0xeaffea);
    return true;  // some deltas found
  }
}

}  // namespace ttfdiff
