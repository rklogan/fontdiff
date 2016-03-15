#include <ft2build.h>
#include FT_GLYPH_H
#include FT_TRUETYPE_TABLES_H

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

#include "ttfdiff/font.h"

namespace ttfdiff {

static FT_Library freeTypeLibrary_;
static bool freeTypeLibraryInited_ = false;
  
Font* Font::Load(const std::string& path) {
  if (!freeTypeLibrary_) {
    // We do not care about thread safety for this tool.
    FT_Init_FreeType(&freeTypeLibrary_);
    freeTypeLibraryInited_ = true;
  }

  FT_Face face = NULL;
  FT_Error error = FT_New_Face(
      freeTypeLibrary_, path.c_str(), 0, &face);
  if (face && !error) {
    return new Font(face);
  } else {
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
    weight_ = os2->usWeightClass;
  }

  TT_Postscript* post =
      static_cast<TT_Postscript*>(FT_Get_Sfnt_Table(face, ft_sfnt_post));
  if (post) {
    italicAngle_ = post->italicAngle;
  }
}


Font::~Font() {
}

bool Font::IsCovering(uint32_t codepoint) const {
  return FT_Get_Char_Index(ft_face_, codepoint) != 0;
}

}  // namespace ttfdiff
