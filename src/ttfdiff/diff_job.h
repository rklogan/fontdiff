#ifndef TTFDIFF_DIFF_JOB_H_
#define TTFDIFF_DIFF_JOB_H_

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_GLYPH_H
#include <unicode/stringpiece.h>

#include "ttfdiff/font_loader.h"
#include "ttfdiff/style_key.h"

typedef struct _cairo cairo_t;
typedef struct _cairo_surface cairo_surface_t;

namespace ttfdiff {

class DiffPage;
class Language;
class Paragraph;
class Style;

class DiffJob {
 public:
  static const FT_F26Dot6 pageWidth, pageHeight, marginWidth;

  DiffJob(const FontCollection* beforeFonts,
	  const FontCollection* afterFonts,
	  const std::string& outputPath);
  ~DiffJob();
  void Render(const std::string& specimenPath);

 private:
  const Language* GetLanguage(const std::string& bcp47);

  struct XMLElement {
    std::string name;
    const Language* language;
    const Style* style;
    Paragraph* paragraph;
  };
  std::vector<XMLElement> xmlElements_;

  void HandleStartElement(
    const std::string& name,
    const std::map<std::string, std::string>& attr);
  void HandleEndElement();
  void HandleCharData(const StringPiece& text);

  const FontCollection* beforeFonts_;
  const FontCollection* afterFonts_;
  cairo_surface_t* pdf_surface_;
  cairo_t* pdf_;
  std::vector<DiffPage*> pages_;  // owner
  std::map<std::string, Language*> languages_;  // owner
  std::vector<Style*> styles_;  // owner
  std::vector<Paragraph*> paragraphs_;  // owner

  friend class ExpatCallbacks;
};

}  // namespace ttfdiff

#endif  // TTFDIFF_DIFF_JOB_H_
