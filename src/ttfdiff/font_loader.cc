#include "openssl/sha.h"
#include "ttfdiff/font.h"
#include "ttfdiff/font_loader.h"

namespace ttfdiff {

FontLoader::FontLoader() {
}

FontLoader::~FontLoader() {
}

Font* FontLoader::Load(const std::string& path) {
  std::string key = GetSHA1(path);
  Font* font = fonts_[key];
  if (!font) {
    font = fonts_[key] = Font::Load(path);
  }
  return font;
}

std::string FontLoader::GetSHA1(const std::string& path) {
  FILE* file = fopen(path.c_str(), "rb");
  if (!file) {
    perror(path.c_str());
    exit(1);
  }

  unsigned char hashValue[SHA_DIGEST_LENGTH];
  const size_t blockSize = 64 * 1024;
  void* block = malloc(blockSize);
  SHA_CTX context;
  SHA1_Init(&context);
  while (!feof(file)) {
    size_t n = fread(block, 1, blockSize, file);
    if (n > 0) {
      SHA1_Update(&context, block, n);
    }
    if (ferror(file)) {
      perror(path.c_str());
      exit(1);
    }
  }
  SHA1_Final(hashValue, &context);
  free(block);
  std::string result;
  for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
    char num[4];
    snprintf(num, sizeof(num), "%02x", hashValue[i]);
    result.append(num);
  }
  return result;
}

}  // namespace ttfdiff
