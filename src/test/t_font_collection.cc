#include "t_font_collection.h"

TEST_CASE("FontCollection: Empty Set")
{
    vector<const fontdiff::Font*> fonts = vector<const fontdiff::Font*>();
    fontdiff::FontCollection fc = fontdiff::FontCollection(fonts);

    const string spec = "";
    fontdiff::Style* style = new fontdiff::Style(NULL, NULL, spec);
    auto font = fontdiff::Font::Load("./src/assets/fonts/OpenSans-BoldItalic.ttf")->at(0);

    REQUIRE(fc.FindFont(0, NULL, NULL) == 0);
    REQUIRE(fc.FindFont(0, style, NULL) == 0);
    REQUIRE(fc.FindFont(0, NULL, font) == 0);
    REQUIRE(fc.FindFont(0, style, font) == 0);

    delete(style); delete(font);
}

TEST_CASE("FontCollection: Single Item in Set")
{
    vector<const fontdiff::Font*> fonts = vector<const fontdiff::Font*>();
    auto font = fontdiff::Font::Load("./src/assets/fonts/OpenSans-BoldItalic.ttf");
    fonts.push_back(font->at(0));
    fontdiff::FontCollection fc = fontdiff::FontCollection(fonts);

    const string spec = "";
    fontdiff::Style* style = new fontdiff::Style(NULL, NULL, spec);

    REQUIRE(fc.FindFont(0, NULL, NULL) == 0);
    REQUIRE(fc.FindFont(0, style, NULL) == 0);
    REQUIRE(fc.FindFont(0, NULL, font->at(0)) == 0);
    REQUIRE(fc.FindFont(100, NULL, font->at(0)) != 0);
    REQUIRE(fc.FindFont(0, style, font->at(0)) == 0);
    REQUIRE(fc.FindFont(100, style, font->at(0)) != 0);

    delete(style); delete(font);
}

TEST_CASE("FontCollection: Multiple Items in a Set")
{
    vector<const fontdiff::Font*> fonts = vector<const fontdiff::Font*>();
    auto font1 = fontdiff::Font::Load("./src/assets/fonts/OpenSans-BoldItalic.ttf");
    auto font2 = fontdiff::Font::Load("./src/assets/fonts/OpenSans-Light.ttf");
    fonts.push_back(font1->at(0));
    fonts.push_back(font2->at(0));
    fontdiff::FontCollection fc = fontdiff::FontCollection(fonts);

    const string spec = "";
    fontdiff::Style* style = new fontdiff::Style(NULL, NULL, spec);

    REQUIRE(fc.FindFont(0, NULL, NULL) == 0);
    REQUIRE(fc.FindFont(0, style, NULL) == 0);
    REQUIRE(fc.FindFont(0, NULL, font1->at(0)) == 0);
    REQUIRE(fc.FindFont(0, NULL, font2->at(0)) == 0);
    REQUIRE(fc.FindFont(100, NULL, font1->at(0)) == font1->at(0));
    REQUIRE(fc.FindFont(100, NULL, font2->at(0)) == font2->at(0));
    REQUIRE(fc.FindFont(0, style, font1->at(0)) == 0);
    REQUIRE(fc.FindFont(100, style, font1->at(0)) == font1->at(0));

    delete(font1); delete(font2); delete(style);
}