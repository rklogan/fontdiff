#ifndef FONTDIFF_FONT_H_
#define FONTDIFF_FONT_H_

#include <string>

#include <ft2build.h>
#include FT_GLYPH_H

typedef struct _cairo_font_face cairo_font_face_t;

namespace fontdiff {

class Font {
 public:
  static Font* Load(const std::string& path);
  ~Font();

  const std::string& GetPostScriptName() const { return psname_; }
  const std::string& GetFamily() const { return family_; }
  const std::string& GetStyle() const { return style_; }
  FT_UShort GetWidth() const { return width_; }
  FT_UShort GetWeight() const { return weight_; }
  FT_Fixed GetItalicAngle() const { return italicAngle_; }
  bool IsCovering(uint32_t codepoint) const;
  FT_Face GetFreetypeFace() const { return ft_face_; }
  cairo_font_face_t* GetCairoFace() const { return cairo_face_; }

 private:
  Font(FT_Face face);

  FT_Face ft_face_;
  cairo_font_face_t* cairo_face_;
  std::string psname_;
  std::string family_;
  std::string style_;
  FT_UShort width_, weight_;
  FT_Fixed italicAngle_;
};
    
}  // namespace fontdiff

#endif  // FONTDIFF_FONT_H_
