#ifndef TTFDIFF_ARGUMENT_PARSER_H_
#define TTFDIFF_ARGUMENT_PARSER_H_

#include <string>
#include <vector>

namespace ttfdiff {

class ArgumentParser {
 public:
  ArgumentParser(int argc, const char** argv);
  ~ArgumentParser();
  const std::vector<std::string>& beforeFonts() const { return beforeFonts_; }
  const std::vector<std::string>& afterFonts() const { return afterFonts_; }
  const std::string& specimen() const { return specimen_; }
  const std::string& output() const { return output_; }

 private:
  void ExplainUsageAndExit(const char* programName);

  std::vector<std::string> beforeFonts_, afterFonts_;
  std::string specimen_;
  std::string output_;
};

}  // namespace ttfdiff

#endif  // TTFDIFF_ARGUMENT_PARSER_H_
