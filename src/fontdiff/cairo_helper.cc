#include "cairo.h"
#include "fontdiff/cairo_helper.h"

namespace fontdiff {

void SetSourceColor(cairo_t* gc, uint32_t color) {
  const double red = ((color >> 16) & 0xff) / 255.0;
  const double green = ((color >> 8) & 0xff) / 255.0;
  const double blue = (color & 0xff) / 255.0;
  cairo_set_source_rgb(gc, red, green, blue);
}

}  // namespace fontdiff
