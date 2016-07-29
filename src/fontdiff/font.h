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

#ifndef FONTDIFF_FONT_H_
#define FONTDIFF_FONT_H_

#include <string>
#include <unordered_map>
#include <vector>

#include <ft2build.h>
#include FT_GLYPH_H
#include FT_MULTIPLE_MASTERS_H

typedef struct _cairo_font_face cairo_font_face_t;
typedef struct hb_font_t hb_font_t;

namespace fontdiff {

class Font {
 public:
  static std::vector<Font*>* Load(const std::string& path);
  ~Font();

  double GetWeightDistance(double weight) const;
  double GetWidthDistance(double width) const;
  FT_Fixed GetItalicAngle() const { return italicAngle_; }
  bool IsCovering(uint32_t codepoint) const;

  FT_Face GetFreeTypeFace(
      double weight, double width, double opticalSize) const;

  hb_font_t* GetHarfBuzzFont(
      double weight, double width, double opticalSize) const;

  cairo_font_face_t* GetCairoFace(
      double weight, double width, double opticalSize) const;

 private:
  static const FT_ULong weightAxisTag, widthAxisTag, opticalSizeAxisTag;
  static double MapWeightClass(FT_UShort weightClass);
  static double MapWidthClass(FT_UShort widthClass);

  struct Instance {
    FT_Face freeTypeFace;
    hb_font_t* harfBuzzFont;
    cairo_font_face_t* cairoFace;
  };

  struct InstanceKey {
    std::vector<FT_Fixed> coords;
    bool operator ==(const InstanceKey& other) const {
      return this->coords == other.coords;
    }
  };

  struct InstanceKeyHasher {
    std::size_t operator()(const InstanceKey& key) const {
      // https://en.wikipedia.org/wiki/Jenkins_hash_function
      std::size_t result = 0;
      for (const FT_Fixed coord : key.coords) {
	result += static_cast<std::size_t>(coord);
	result += (result << 10);
	result ^= (result >> 6);
      }
      result += (result << 3);
      result ^= (result >> 11);
      result += (result << 15);
      return result;
    }
  };

  Font(const std::string& filepath, int index);

  const Instance* GetInstance(
      double weight, double width, double opticalSize) const;

  bool GetInstanceKey(
      double weight, double width, double opticalSize,
      InstanceKey* key) const;

  const std::string filepath_;
  const int fontIndex_;
  FT_MM_Var* variations_;
  Instance defaultInstance_;
  mutable std::unordered_map<InstanceKey, Instance*, InstanceKeyHasher>
      instances_;

  double minWidth_, defaultWidth_, maxWidth_;     // 50..200
  double minWeight_, defaultWeight_, maxWeight_;  // 100..1000
  FT_Fixed italicAngle_;
};

}  // namespace fontdiff

#endif  // FONTDIFF_FONT_H_
