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
  void ShowVersionAndExit();
  void ExplainUsageAndExit(const char* programName);

  std::vector<std::string> beforeFonts_, afterFonts_;
  std::string specimen_;
  std::string output_;
};

}  // namespace fontdiff

#endif  // FONTDIFF_ARGUMENT_PARSER_H_
