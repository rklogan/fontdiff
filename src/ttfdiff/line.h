#ifndef TTFDIFF_LINE_H_
#define TTFDIFF_LINE_H_

#include <stdint.h>
#include <vector>

#include <ft2build.h>
#include FT_GLYPH_H

typedef struct _cairo cairo_t;

namespace ttfdiff {

class ShapedText;

class Line {
 public:
  Line(FT_F26Dot6 width);
  ~Line();
  void SetBackgroundColor(uint32_t rgb) { backgroundColor_ = rgb; }
  void AddShapedText(const ShapedText* text, int32_t start, int32_t limit);
  FT_F26Dot6 GetAscender() const { return ascender_; }
  FT_F26Dot6 GetDescender() const { return descender_; }
  FT_F26Dot6 GetWidth() const { return width_; }
  void Render(cairo_t* gc, FT_F26Dot6 x, FT_F26Dot6 y) const;
  void RenderHighlights(cairo_t* gc, FT_F26Dot6 x, FT_F26Dot6 y) const;

 private:
  struct Run {
    const ShapedText* text;
    FT_F26Dot6 x;
    int32_t start, limit;
  };
  std::vector<Run> runs_;
  FT_F26Dot6 width_, xAdvance_, ascender_, descender_;
  uint32_t backgroundColor_;
};


}  // namespace ttfdiff

#endif  // TTFDIFF_LINE_H_
