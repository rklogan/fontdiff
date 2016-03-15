#include <ft2build.h>
#include FT_GLYPH_H
#include FT_TRUETYPE_TABLES_H

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#include <cairo/cairo-pdf.h>
#include <expat.h>
#include <unicode/stringpiece.h>

#include "ttfdiff/diff_job.h"
#include "ttfdiff/font.h"
#include "ttfdiff/language.h"
#include "ttfdiff/paragraph.h"
#include "ttfdiff/style.h"


namespace ttfdiff {

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

DiffJob::DiffJob(const std::vector<std::string>& beforeFonts,
		 const std::vector<std::string>& afterFonts,
		 const std::string& outputPath)
  : pdf_surface_(
        cairo_pdf_surface_create(outputPath.c_str(),
				 pageWidth / 64.0, pageHeight / 64.0)),
    pdf_(cairo_create(pdf_surface_)) {
}
  
DiffJob::~DiffJob() {
  cairo_destroy(pdf_);
  cairo_surface_destroy(pdf_surface_);
  for (auto style : styles_) delete style;
  for (auto lang : languages_) delete lang.second;
  for (auto p : paragraphs_) delete p;
}


const Font* DiffJob::FindFont(
    UChar32 codepoint, const Style* style, bool isBefore,
    const Font* lastFont) const {
  if (lastFont && lastFont->IsCovering(codepoint)) {
    return lastFont;
  }

  const StyleFontMap& fonts = isBefore ? beforeFonts_ : afterFonts_;
  for (auto p : fonts) {
    const Font* font = p.second;
    if (font->IsCovering(codepoint)) {
      return font;
    }
    printf("FindFont: Considering %s\n", font->GetPostScriptName().c_str());
  }

  return NULL;
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
  } else {
    element.style = cur.style;
  }
  if (name == "div" || name == "p" || name == "h1") {
    paragraphs_.push_back(new Paragraph());
  }
  element.paragraph = paragraphs_.back();
  xmlElements_.push_back(element);
  printf("StartElement %s\n", name.c_str());
}

void DiffJob::HandleEndElement() {
  xmlElements_.pop_back();
}

void DiffJob::HandleCharData(const StringPiece& text) {
  const XMLElement& e = xmlElements_.back();
  e.paragraph->AppendSpan(text, e.style);
}

void DiffJob::Render(const std::string& specimenPath) {
  xmlElements_.clear();
  XMLElement rootElement;
  rootElement.name = "";
  rootElement.language = GetLanguage("und");
  styles_.push_back(new Style(NULL, rootElement.language, ""));
  rootElement.style = styles_.back();
  paragraphs_.push_back(new Paragraph());
  rootElement.paragraph = paragraphs_.back();
  xmlElements_.push_back(rootElement);

  XML_Parser parser = XML_ParserCreate("utf-8");
  ExpatCallbacks::Install(parser, this);
  FILE* file = fopen(specimenPath.c_str(), "rb");
  if (!file) {
    perror(specimenPath.c_str());
    exit(1);
  }
  const size_t blockSize = 64 * 1024;
  void* block = malloc(blockSize);
  while (!feof(file)) {
    size_t n = fread(block, 1, blockSize, file);
    if (ferror(file)) {
      perror(specimenPath.c_str());
      exit(1);
    }
    if (XML_Parse(parser, reinterpret_cast<const char*>(block), n, feof(file))
	!= XML_STATUS_OK) {
      fflush(stdout);
      fprintf(stderr, "%s: %s\n", specimenPath.c_str(),
	     XML_ErrorString(XML_GetErrorCode(parser)));
      exit(1);
    }
  }
  free(block);
  fclose(file);
  for (auto p : paragraphs_) {
    p->Layout();
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

}  // namespace ttfdiff
