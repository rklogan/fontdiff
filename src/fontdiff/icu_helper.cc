#include <stdio.h>
#include <stdlib.h>
#include "fontdiff/icu_helper.h"

namespace fontdiff {

void CheckUErrorCode(const UErrorCode& err) {
  if (U_FAILURE(err)) {
    fflush(stdout);
    fprintf(stderr, "ICU error: %s\n", u_errorName(err));
    exit(2);
  }
}

}  // namespace fontdiff
