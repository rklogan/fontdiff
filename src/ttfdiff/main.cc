#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "ttfdiff/argument_parser.h"
#include "ttfdiff/diff_job.h"

int main(int argc, const char** argv) {
  ttfdiff::ArgumentParser args(argc, argv);  
  ttfdiff::DiffJob job(args.beforeFonts(), args.afterFonts(), args.output());
  job.Render(args.specimen());
}
