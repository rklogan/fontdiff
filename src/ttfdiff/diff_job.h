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

class Language;
class Page;
class Paragraph;
class Style;

class DiffJob {
 public:
  static const FT_F26Dot6 pageWidth, pageHeight, marginWidth;

  DiffJob(const FontCollection* beforeFonts,
	  const FontCollection* afterFonts,
	  const std::string& outputPath);
  ~DiffJob();
  Page* GetCurrentPage() { return pages_.back(); }
  Page* AddPage();
  void Render(const std::string& specimenPath);
  bool HasDiffs() const { return has_diffs_; }
  void SetHasDiffs() { has_diffs_ = true; }

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

  bool has_diffs_;
  const FontCollection* beforeFonts_;
  const FontCollection* afterFonts_;
  cairo_surface_t* pdf_surface_;  // owned
  cairo_t* pdf_;  // owned
  std::map<std::string, Language*> languages_;  // owned
  std::vector<Style*> styles_;  // owned
  std::vector<Paragraph*> paragraphs_;  // owned
  std::vector<Page*> pages_;  // owned

  friend class ExpatCallbacks;
};

}  // namespace ttfdiff

#endif  // TTFDIFF_DIFF_JOB_H_
