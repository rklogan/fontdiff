/*
 * Copyright 2017 Google Inc. All rights reserved.
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

#include <string>
#include <string.h>
#include "fontdiff/version.h"

namespace fontdiff {

#ifndef FONTDIFF_VERSION
#define FONTDIFF_VERSION 0
#endif
#define VERSION_str(s) #s
#define VERSION_xstr(s) VERSION_str(s)

std::string GetVersion() {
  const char* version = VERSION_xstr(FONTDIFF_VERSION);
  if (version && version[0] == 'v') {
    ++version;
  }
  if (!version || *version == '\0' || !strcmp(version, "0")) {
    version = NULL;
  }
  return version ? std::string(version) : std::string();
}

}  // namespace fontdiff
