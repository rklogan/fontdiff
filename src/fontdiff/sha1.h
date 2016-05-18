#ifndef FONTDIFF_SHA1_H_
#define FONTDIFF_SHA1_H_

#include <string>

namespace fontdiff {
std::string GetFileSHA1(const std::string& filepath);
}

#endif  // FONTDIFF_SHA1_H_
