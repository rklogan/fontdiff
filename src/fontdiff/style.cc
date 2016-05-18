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

#include <cstdlib>
#include <string>
#include <vector>
#include "fontdiff/font.h"
#include "fontdiff/language.h"
#include "fontdiff/style.h"

namespace fontdiff {

static void SplitString(const std::string& text, char sep,
			std::vector<std::string>* result) {
  std::size_t start = 0, limit = 0;
  while ((limit = text.find(sep, start)) != std::string::npos) {
    result->push_back(text.substr(start, limit - start));
    start = limit + 1;
  }
  result->push_back(text.substr(start));
}

void TrimWhitespace(std::string* str) {
  static const char* whitespace = " \t\f\v\n\r";
  const std::size_t start = str->find_first_not_of(whitespace);
  if (start == std::string::npos) {
    str->clear();
    return;
  }
  str->substr(start).swap(*str);
  const std::size_t end = str->find_last_not_of(whitespace);
  if (end != std::string::npos) {
    str->erase(end + 1);
  }
}

Style::Style(const Style* parent, const Language* language,
	     const std::string& spec)
  : language_(language),
    fontSize_(parent ? parent->fontSize_ : 12.0),
    fontWeight_(parent ? parent->fontWeight_ : 400) {
  std::vector<std::string> specItems;
  SplitString(spec, ';', &specItems);
  for (const std::string item : specItems) {
    std::size_t colonPos = item.find(':');
    if (colonPos == std::string::npos) {
      continue;
    }
    std::string key = item.substr(0, colonPos);
    std::string value = item.substr(colonPos + 1);
    TrimWhitespace(&key);
    TrimWhitespace(&value);
    SetProperty(key, value);
  }
}

Style::~Style() {
}

int32_t Style::GetFontScore(const Font& font) const {
  // TODO: Implement properly, based on style specification.
  int32_t delta = 0;
  delta += std::abs(font.GetWeight() - static_cast<int32_t>(fontWeight_));
  if (font.GetItalicAngle() != 0) {
    delta += 50;
  }
  return delta;
}

static double GetBolderWeight(double weight) {
  if (weight < 400) {
    return 400;
  } else if (weight < 600) {
    return 700;
  } else {
    return 900;
  }
}

static double GetLighterWeight(double weight) {
  if (weight < 600) {
    return 100;
  } else if (weight < 800) {
    return 400;
  } else {
    return 700;
  }
}

void Style::SetProperty(const std::string& key, const std::string& value) {
  std::size_t len = value.size();
  
  if (key == "font-size") {
    if (value.find("pt") == len - 2) {
      SetFontSize(std::atof(value.substr(0, len - 2).c_str()));
    } else if (value.find("px") == len - 2) {
      SetFontSize(std::atof(value.substr(0, len - 2).c_str()) * 12.0 / 16.0);
    } else if (value.find("em") == len - 2) {
      const double val = std::atof(value.substr(0, len - 2).c_str());
      SetFontSize(val * fontSize_);
    } else if (value.find("%") == len - 1) {
      const double factor = std::atof(value.substr(0, len - 1).c_str());
      SetFontSize(fontSize_ * factor / 100.0);
    }
  }

  if (key == "font-weight") {
    if (value == "normal") {
      SetFontWeight(400);
    } else if (value == "bold") {
      SetFontWeight(700);
    } else if (value == "bolder") {
      SetFontWeight(GetBolderWeight(fontWeight_));
    } else if (value == "lighter") {
      SetFontWeight(GetLighterWeight(fontWeight_));
    } else {
      SetFontWeight(std::atof(value.c_str()));
    }
  }
}

void Style::SetFontSize(double size) {
  if (size > 0 && size < 500) {
    fontSize_ = size;
  }
}

void Style::SetFontWeight(double weight) {
  if (weight >= 100 && weight <= 900) {
    fontWeight_ = weight;
  }
}

}  // namespace fontdiff
