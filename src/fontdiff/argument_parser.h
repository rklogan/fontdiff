#ifndef FONTDIFF_ARGUMENT_PARSER_H_
#define FONTDIFF_ARGUMENT_PARSER_H_

#include <string>
#include <vector>

namespace fontdiff {

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

}  // namespace fontdiff

#endif  // FONTDIFF_ARGUMENT_PARSER_H_
