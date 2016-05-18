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

#ifndef FONTDIFF_DIFF_JOB_H_
#define FONTDIFF_DIFF_JOB_H_

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_GLYPH_H
#include <unicode/stringpiece.h>

#include "fontdiff/font_loader.h"
#include "fontdiff/style_key.h"

typedef struct _cairo cairo_t;
typedef struct _cairo_surface cairo_surface_t;

namespace fontdiff {

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

}  // namespace fontdiff

#endif  // FONTDIFF_DIFF_JOB_H_
