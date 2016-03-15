#ifndef TTFDIFF_ICU_HELPER_H_
#define TTFDIFF_ICU_HELPER_H_

#include <unicode/errorcode.h>

namespace ttfdiff {
void CheckUErrorCode(const UErrorCode& err);  
}  // namespace ttfdiff

#endif // TTFDIFF_ICU_HELPER_H_
