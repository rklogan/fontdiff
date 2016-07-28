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
#include FT_TRUETYPE_TABLES_H

#include "cairo.h"
#include "cairo-ft.h"

#include "fontdiff/font.h"

namespace fontdiff {

static FT_Library freeTypeLibrary_ = NULL;
static bool freeTypeLibraryInited_ = false;

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
    FT_Face face = NULL;
    FT_Error error = FT_New_Face(
        freeTypeLibrary_, path.c_str(), i, &face);
    if (face && !error) {
      result->push_back(new Font(face));
    }
  }

  if (result->size() > 0) {
    return result;
  } else {
    delete result;
    return NULL;
  }
}

Font::Font(FT_Face face)
  : ft_face_(face),
    cairo_face_(
        cairo_ft_font_face_create_for_ft_face(face, FT_LOAD_NO_HINTING)),
    family_(face->family_name),
    style_(face->style_name),
    width_(5), weight_(400), italicAngle_(0) {
  const char* psname = FT_Get_Postscript_Name(ft_face_);
  if (psname) {
    psname_ = psname;
  }
  if (family_.find("Noto Sans ") == 0 &&
      family_ != "Noto Sans Mono") {
    family_ = "Noto Sans";
  }
  if (family_.find("Noto Serif ") == 0) {
    family_ = "Noto Serif";
  }

  TT_OS2* os2 = static_cast<TT_OS2*>(FT_Get_Sfnt_Table(face, ft_sfnt_os2));
  if (os2) {
    width_ = os2->usWidthClass;
    weight_ = MapWeightClass(os2->usWeightClass);
  }

  TT_Postscript* post =
      static_cast<TT_Postscript*>(FT_Get_Sfnt_Table(face, ft_sfnt_post));
  if (post) {
    italicAngle_ = post->italicAngle;
  }
}


Font::~Font() {
}


// Work around values that can be found in OS/2 tables of some old fonts.
// Logic taken from FontConfig.
FT_UShort Font::MapWeightClass(FT_UShort weight) {
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

bool Font::IsCovering(uint32_t codepoint) const {
  return FT_Get_Char_Index(ft_face_, codepoint) != 0;
}

}  // namespace fontdiff
