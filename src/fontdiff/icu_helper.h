#ifndef FONTDIFF_ICU_HELPER_H_
#define FONTDIFF_ICU_HELPER_H_

#include <unicode/errorcode.h>

namespace fontdiff {
void CheckUErrorCode(const UErrorCode& err);  
}  // namespace fontdiff

#endif // FONTDIFF_ICU_HELPER_H_
