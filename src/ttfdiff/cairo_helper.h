#ifndef TTFDIFF_CAIRO_HELPER_H_
#define TTFDIFF_CAIRO_HELPER_H_

#include <stdint.h>
typedef struct _cairo cairo_t;

namespace ttfdiff {
void SetSourceColor(cairo_t* gc, uint32_t rgb);
}  // namespace ttfdiff

#endif  // TTFDIFF_CAIRO_HELPER_H_
