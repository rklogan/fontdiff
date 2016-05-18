#include "fontdiff/font.h"
#include "fontdiff/font_collection.h"
#include "fontdiff/font_loader.h"
#include "fontdiff/sha1.h"

namespace fontdiff {

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

}  // namespace fontdiff
