#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <string>

#include "ttfdiff/argument_parser.h"
#include "ttfdiff/diff_job.h"
#include "ttfdiff/font_collection.h"
#include "ttfdiff/font_loader.h"

int main(int argc, const char** argv) {
  ttfdiff::ArgumentParser args(argc, argv);
  ttfdiff::FontLoader loader;
  std::unique_ptr<ttfdiff::FontCollection> beforeFonts(
      loader.LoadCollection(args.beforeFonts()));
  std::unique_ptr<ttfdiff::FontCollection> afterFonts(
      loader.LoadCollection(args.afterFonts()));
  ttfdiff::DiffJob job(beforeFonts.get(), afterFonts.get(), args.output());
  job.Render(args.specimen());
  return job.HasDiffs() ? 1 : 0;
}
