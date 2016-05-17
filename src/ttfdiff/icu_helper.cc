#include <stdio.h>
#include <stdlib.h>
#include "ttfdiff/icu_helper.h"

namespace ttfdiff {

void CheckUErrorCode(const UErrorCode& err) {
  if (U_FAILURE(err)) {
    fflush(stdout);
    fprintf(stderr, "ICU error: %s\n", u_errorName(err));
    exit(2);
  }
}

}  // namespace ttfdiff
