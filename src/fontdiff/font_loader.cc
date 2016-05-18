#include "ttfdiff/font.h"
#include "ttfdiff/font_collection.h"
#include "ttfdiff/font_loader.h"
#include "ttfdiff/sha1.h"

namespace ttfdiff {

FontLoader::FontLoader() {
}

FontLoader::~FontLoader() {
}

Font* FontLoader::Load(const std::string& path) {
  std::string key = GetFileSHA1(path);
  Font* font = fonts_[key];
  if (!font) {
    font = fonts_[key] = Font::Load(path);
  }
  return font;
}

FontCollection* FontLoader::LoadCollection(
    const std::vector<std::string>& paths) {
  std::vector<const Font*> fonts;
  for (int i = 0; i < paths.size(); ++i) {
    fonts.push_back(Load(paths[i]));
  }
  return new FontCollection(fonts);
}

}  // namespace ttfdiff
