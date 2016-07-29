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

#include <ft2build.h>
#include FT_GLYPH_H
#include FT_MULTIPLE_MASTERS_H
#include FT_TRUETYPE_TABLES_H
#include FT_TYPES_H

#include "cairo.h"
#include "cairo-ft.h"
#include "hb.h"
#include "hb-ft.h"

#include "fontdiff/font.h"

namespace fontdiff {

static FT_Library freeTypeLibrary_ = NULL;
static bool freeTypeLibraryInited_ = false;

const FT_ULong Font::weightAxisTag = FT_MAKE_TAG('w', 'g', 'h', 't');
const FT_ULong Font::widthAxisTag = FT_MAKE_TAG('w', 'd', 't', 'h');
const FT_ULong Font::opticalSizeAxisTag = FT_MAKE_TAG('o', 'p', 's', 'z');

std::vector<Font*>* Font::Load(const std::string& path) {
  if (!freeTypeLibraryInited_) {
    // We do not care about thread safety for this tool.
    FT_Init_FreeType(&freeTypeLibrary_);
    freeTypeLibraryInited_ = true;
  }

  FT_Long numFaces = 0;
  FT_Face face = NULL;
  FT_Error error = FT_New_Face(freeTypeLibrary_, path.c_str(), -1, &face);
  if (face) {
    if (!error) {
      numFaces = face->num_faces;
    }
    FT_Done_Face(face);
  }

  std::vector<Font*>* result = new std::vector<Font*>();
  for (FT_Long i = 0; i < numFaces; ++i) {
    result->push_back(new Font(path, i));
  }

  if (result->size() > 0) {
    return result;
  } else {
    delete result;
    return NULL;
  }
}

Font::Font(const std::string& filepath, int index)
  : filepath_(filepath), fontIndex_(index),
    ft_face_(NULL), ft_variations_(NULL), harfBuzzFont_(NULL),
    defaultWidth_(100), defaultWeight_(400), italicAngle_(0) {
  FT_Error error =
      FT_New_Face(freeTypeLibrary_, filepath.c_str(), index, &ft_face_);
  if (error) {
    fprintf(stderr, "cannot load font from %s\n", filepath.c_str());
    exit(2);
  }

  harfBuzzFont_ = hb_ft_font_create(ft_face_, NULL);
  hb_ft_font_set_load_flags(harfBuzzFont_, FT_LOAD_NO_HINTING);

  FT_Face face = ft_face_;
  cairo_face_ =
      cairo_ft_font_face_create_for_ft_face(face, FT_LOAD_NO_HINTING);
  if (FT_HAS_MULTIPLE_MASTERS(face)) {
    FT_Get_MM_Var(face, &ft_variations_);
  }

  TT_OS2* os2 = static_cast<TT_OS2*>(FT_Get_Sfnt_Table(face, ft_sfnt_os2));
  if (os2) {
    defaultWidth_ = MapWidthClass(os2->usWidthClass);
    defaultWeight_ = MapWeightClass(os2->usWeightClass);
  }
  minWidth_ = maxWidth_ = defaultWidth_;
  minWeight_ = maxWeight_ = defaultWeight_;

  TT_Postscript* post =
      static_cast<TT_Postscript*>(FT_Get_Sfnt_Table(face, ft_sfnt_post));
  if (post) {
    italicAngle_ = post->italicAngle;
  }

  if (ft_variations_) {
    for (int i = 0; i < ft_variations_->num_axis; ++i) {
      const FT_Var_Axis& axis = ft_variations_->axis[i];
      const double defaultValue = axis.def / (double) (1 << 16);
      const double minValue = axis.minimum / (double) (1 << 16);
      const double maxValue = axis.maximum / (double) (1 << 16);
      const double minMultiplier = minValue / defaultValue;
      const double maxMultiplier = maxValue / defaultValue;
      if (axis.tag == weightAxisTag) {
        minWeight_ = minMultiplier * defaultWeight_;
        maxWeight_ = maxMultiplier * defaultWeight_;
      } else if (axis.tag == widthAxisTag) {
        minWidth_ = minMultiplier * defaultWidth_;
        maxWidth_ = maxMultiplier * defaultWidth_;
      }
    }
  }
}


Font::~Font() {
  if (ft_variations_) {
    free(ft_variations_);
  }
  if (harfBuzzFont_) {
    hb_font_destroy(harfBuzzFont_);
  }
  if (ft_face_) {
    FT_Done_Face(ft_face_);
  }
}


// Work around values that can be found in OS/2 tables of some old fonts.
// Logic taken from FontConfig.
double Font::MapWeightClass(FT_UShort weight) {
  if (weight <= 9) {
    switch (weight) {
    case 0: return 100;
    case 1: return 100;
    case 2: return 160;
    case 3: return 240;
    case 4: return 320;
    case 5: return 400;
    case 6: return 550;
    case 7: return 700;
    case 8: return 800;
    case 9: return 900;
    }
  }

  if (weight < 100) {
    return 100;
  } else if (weight > 1000) {
    return 1000;
  } else {
    return weight;
  }
}

double Font::MapWidthClass(FT_UShort widthClass) {
  // https://www.microsoft.com/typography/otspec/os2.htm#wdc
  switch (widthClass) {
  case 1: return 50;
  case 2: return 62.5;
  case 3: return 75;
  case 4: return 87.5;
  case 5: return 100;
  case 6: return 112.5;
  case 7: return 125;
  case 8: return 150;
  case 9: return 200;
  default: return 100;
  }
}

double Font::GetWeightDistance(double weight) const {
  if (weight < minWeight_) {
    return minWeight_ - weight;
  } else if (weight > maxWeight_) {
    return weight - maxWeight_;
  } else {
    return 0;
  }
}

double Font::GetWidthDistance(double width) const {
  if (width < minWidth_) {
    return minWidth_ - width;
  } else if (width > maxWidth_) {
    return width - maxWidth_;
  } else {
    return 0;
  }
}

bool Font::IsCovering(uint32_t codepoint) const {
  return FT_Get_Char_Index(ft_face_, codepoint) != 0;
}

}  // namespace fontdiff
