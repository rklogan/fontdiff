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

#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <string>

#include "fontdiff/argument_parser.h"
#include "fontdiff/diff_job.h"
#include "fontdiff/font_collection.h"
#include "fontdiff/font_loader.h"

int main(int argc, const char** argv) {
  fontdiff::ArgumentParser args(argc, argv);
  fontdiff::FontLoader loader;
  std::unique_ptr<fontdiff::FontCollection> beforeFonts(
      loader.LoadCollection(args.beforeFonts()));
  std::unique_ptr<fontdiff::FontCollection> afterFonts(
      loader.LoadCollection(args.afterFonts()));
  fontdiff::DiffJob job(beforeFonts.get(), afterFonts.get(), args.output());
  job.Render(args.specimen());
  return job.HasDiffs() ? 1 : 0;
}
