#ifndef TTFDIFF_FONT_LOADER_H_
#define TTFDIFF_FONT_LOADER_H_

#include <map>
#include <string>

namespace ttfdiff {

class Font;

class FontLoader {
 public:
  FontLoader();
  ~FontLoader();
  Font* Load(const std::string& path);

 private:
  static std::string GetSHA1(const std::string& filepath);
  std::map<std::string, Font*> fonts_;
};

}  // namespace ttfdiff

#endif  // TTFDIFF_FONT_LOADER_H_
