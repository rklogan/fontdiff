#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ttfdiff/argument_parser.h"

namespace ttfdiff {

ArgumentParser::ArgumentParser(int argc, const char** argv) {
  enum CurOption { NONE, BEFORE, AFTER, SPECIMEN, OUT };
  CurOption cur = NONE;
  for (int i = 1; i < argc; ++i) {
    const char* arg = argv[i];
    if (!strcmp(arg, "--before")) {
      cur = BEFORE;
      continue;
    } else if (!strcmp(arg, "--after")) {
      cur = AFTER;
      continue;
    } else if (!strcmp(arg, "--specimen")) {
      cur = SPECIMEN;
      continue;
    } else if (!strcmp(arg, "--out")) {
      cur = OUT;
      continue;
    }

    if (argv[i][0] == '-') {
      ExplainUsageAndExit(argv[0]);
    }

    switch (cur) {
      case BEFORE: beforeFonts_.push_back(arg); break;
    case AFTER: afterFonts_.push_back(arg); break;
    case SPECIMEN: specimen_.assign(arg); break;
    case OUT: output_.assign(arg); break;
    default:
      ExplainUsageAndExit(argv[0]);
    }
  }
  if (output_.empty() || specimen_.empty()) {
    ExplainUsageAndExit(argv[0]);
  }
}

ArgumentParser::~ArgumentParser() {
}

void ArgumentParser::ExplainUsageAndExit(const char* programName) {
  fprintf(stderr,
          "Usage: %s --before a.ttf --after b.ttf "
          "--specimen specimen.html --out out.pdf\n",
          programName);
  exit(1);
}

}  // namespace ttfdiff
