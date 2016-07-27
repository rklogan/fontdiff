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

#include <cairo.h>
#include <cairo-ft.h>
#include <cairo-pdf.h>
#include <expat.h>
#include <unicode/stringpiece.h>

#include "fontdiff/diff_job.h"
#include "fontdiff/font.h"
#include "fontdiff/language.h"
#include "fontdiff/page.h"
#include "fontdiff/paragraph.h"
#include "fontdiff/style.h"


namespace fontdiff {

const FT_F26Dot6 DiffJob::pageWidth = 592 * 64;  // DIN A4
const FT_F26Dot6 DiffJob::pageHeight = 842 * 64;
const FT_F26Dot6 DiffJob::marginWidth = 50 * 64;

class ExpatCallbacks {
 public:
  static void Install(XML_Parser parser, DiffJob* job);

 private:
  static void HandleStartElement(void* userData,
                                 const XML_Char* name,
                                 const XML_Char** attrs);
  static void HandleEndElement(void* userData, const XML_Char* name);
  static void HandleCharData(void* userData, const XML_Char *s, int len);
};

DiffJob::DiffJob(const FontCollection* beforeFonts,
                 const FontCollection* afterFonts)
  : has_diffs_(false),
    beforeFonts_(beforeFonts), afterFonts_(afterFonts) {
  pages_.push_back(new Page());
}

DiffJob::~DiffJob() {
  for (auto style : styles_) delete style;
  for (auto lang : languages_) delete lang.second;
  for (auto p : paragraphs_) delete p;
  for (auto p : pages_) delete p;
}

void DiffJob::WritePDF(const std::string& filepath) {
  cairo_surface_t* pdf_surface =
      cairo_pdf_surface_create(filepath.c_str(),
                               pageWidth / 64.0, pageHeight / 64.0);
  cairo_t* pdf = cairo_create(pdf_surface);
  for (const Page* page : pages_) {
    page->Render(pdf);
    cairo_show_page(pdf);
  }
  cairo_destroy(pdf);
  cairo_surface_destroy(pdf_surface);
}

Page* DiffJob::AddPage() {
  Page* page = new Page();
  pages_.push_back(page);
  return page;
}

void DiffJob::HandleStartElement(
    const std::string& name,
    const std::map<std::string, std::string>& attr) {
  const XMLElement& cur = xmlElements_.back();
  XMLElement element;
  element.name = name;
  auto xmlLangIter = attr.find("xml:lang");
  auto langIter = attr.find("lang");
  if (xmlLangIter != attr.end()) {
    element.language = GetLanguage(xmlLangIter->second);
  } else if (langIter != attr.end()) {
    element.language = GetLanguage(langIter->second);
  } else {
    element.language = cur.language;
  }
  auto styleIter = attr.find("style");
  if (styleIter != attr.end()) {
    styles_.push_back(
        new Style(cur.style, element.language, styleIter->second));
    element.style = styles_.back();
  } else if (cur.style->GetLanguage() != element.language) {
    styles_.push_back(new Style(cur.style, element.language, ""));
    element.style = styles_.back();
  } else {
    element.style = cur.style;
  }
  if (name == "div" || name == "p" || name == "h1") {
    paragraphs_.push_back(new Paragraph(this, beforeFonts_, afterFonts_));
  }
  element.paragraph = paragraphs_.back();
  xmlElements_.push_back(element);
}

void DiffJob::HandleEndElement() {
  xmlElements_.pop_back();
}

void DiffJob::HandleCharData(const StringPiece& text) {
  const XMLElement& e = xmlElements_.back();
  e.paragraph->AppendSpan(text, e.style);
}

void DiffJob::RenderHTML(const std::string& filepath) {
  xmlElements_.clear();
  XMLElement rootElement;
  rootElement.name = "";
  rootElement.language = GetLanguage("und");
  styles_.push_back(new Style(NULL, rootElement.language, ""));
  rootElement.style = styles_.back();
  paragraphs_.push_back(new Paragraph(this, beforeFonts_, afterFonts_));
  rootElement.paragraph = paragraphs_.back();
  xmlElements_.push_back(rootElement);

  XML_Parser parser = XML_ParserCreate("utf-8");
  ExpatCallbacks::Install(parser, this);
  FILE* file = fopen(filepath.c_str(), "rb");
  if (!file) {
    perror(filepath.c_str());
    exit(2);
  }
  const size_t blockSize = 64 * 1024;
  void* block = malloc(blockSize);
  while (!feof(file)) {
    size_t n = fread(block, 1, blockSize, file);
    if (ferror(file)) {
      perror(filepath.c_str());
      exit(2);
    }
    if (XML_Parse(parser, reinterpret_cast<const char*>(block), n, feof(file))
        != XML_STATUS_OK) {
      fflush(stdout);
      fprintf(stderr, "%s: %s\n", filepath.c_str(),
              XML_ErrorString(XML_GetErrorCode(parser)));
      exit(2);
    }
  }
  free(block);
  fclose(file);
  for (auto p : paragraphs_) {
    p->Layout(pageWidth - 2 * marginWidth);
  }
}

const Language* DiffJob::GetLanguage(const std::string& bcp47) {
  const Language* lang = languages_[bcp47];
  if (!lang) {
    lang = languages_[bcp47] = new Language(bcp47);
  }
  return lang;
}

void ExpatCallbacks::Install(XML_Parser parser, DiffJob* job) {
  XML_SetUserData(parser, job);
  XML_SetElementHandler(parser, ExpatCallbacks::HandleStartElement,
                        ExpatCallbacks::HandleEndElement);
  XML_SetCharacterDataHandler(parser, ExpatCallbacks::HandleCharData);
}

void ExpatCallbacks::HandleStartElement(
    void* userData,
    const XML_Char* name,
    const XML_Char** attrs) {
  DiffJob* job = reinterpret_cast<DiffJob*>(userData);
  std::map<std::string, std::string> attrMap;
  for (const XML_Char** attr = attrs; *attr != NULL; attr += 2) {
    attrMap[attr[0]] = attr[1];
  }
  job->HandleStartElement(name, attrMap);
}

void ExpatCallbacks::HandleEndElement(void* userData, const XML_Char* name) {
  DiffJob* job = reinterpret_cast<DiffJob*>(userData);
  job->HandleEndElement();
}

void ExpatCallbacks::HandleCharData(void* userData,
                                    const XML_Char* text, int len) {
  DiffJob* job = reinterpret_cast<DiffJob*>(userData);
  job->HandleCharData(icu::StringPiece(text, len));
}

}  // namespace fontdiff
