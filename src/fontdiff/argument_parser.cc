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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fontdiff/argument_parser.h"

namespace fontdiff {

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
    } else if (!strcmp(arg, "--version")) {
      ShowVersionAndExit();
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

void ArgumentParser::ShowVersionAndExit() {
#ifndef FONTDIFF_VERSION
#define FONTDIFF_VERSION 0
#endif
#define VERSION_str(s) #s
#define VERSION_xstr(s) VERSION_str(s)

  const char* version = VERSION_xstr(FONTDIFF_VERSION);

  if (!version || *version == '\0' || !strcmp(version, "0")) {
    version = "(unreleased development version)";
  }
  printf("fontdiff %s\n", version);
  exit(0);
}

void ArgumentParser::ExplainUsageAndExit(const char* programName) {
  fprintf(stderr,
          "Usage: %s --before a.ttf --after b.ttf "
          "--specimen specimen.html --out out.pdf\n",
          programName);
  exit(2);
}

}  // namespace fontdiff
