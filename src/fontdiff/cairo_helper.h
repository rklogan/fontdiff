#ifndef FONTDIFF_CAIRO_HELPER_H_
#define FONTDIFF_CAIRO_HELPER_H_

#include <stdint.h>
typedef struct _cairo cairo_t;

namespace fontdiff {
void SetSourceColor(cairo_t* gc, uint32_t rgb);
}  // namespace fontdiff

#endif  // FONTDIFF_CAIRO_HELPER_H_
