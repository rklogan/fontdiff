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
