#include "t_font.h"


// Test Solutions
const vector<string> pathToFonts = vector<string>({
    "./src/assets/fonts/OpenSans-BoldItalic.ttf",
    "./src/assets/fonts/OpenSans-Bold.ttf",
    "./src/assets/fonts/OpenSans-ExtraBold.ttf",
    "./src/assets/fonts/OpenSans-ExtraBoldItalic.ttf",
    "./src/assets/fonts/OpenSans-Italic.ttf",
    "./src/assets/fonts/OpenSans-Light.ttf",
    "./src/assets/fonts/OpenSans-LightItalic.ttf",
    "./src/assets/fonts/OpenSans-Regular.ttf",
    "./src/assets/fonts/OpenSans-Semibold.ttf",
    "./src/assets/fonts/OpenSans-SemiboldItalic.ttf",
    "./src/assets/fonts/OpenSans-BoldItalic.ttf"
});

const unsigned int weightDistances1[2][11] = {
    {700, 700, 800, 800, 400, 300, 300, 400, 600, 600, 700},
    {686, 686, 786, 786, 386, 286, 286, 386, 586, 586, 686}
};

const double weightDistances2[11] 
    = {690.4, 690.4, 790.4, 790.4, 390.4, 290.4, 290.4, 390.4, 590.4, 590.4, 690.4};

const int italicAngles[11] =
    {-786432, 0, 0, -786432, -786432, 0, -786432, 0, 0, -786432, -786432};

TEST_CASE("Font::Load Single Argument")
{
    for(auto it = pathToFonts.begin(); it != pathToFonts.end(); ++it)
    {
        auto font = fontdiff::Font::Load(*it);
        REQUIRE(font != NULL);
        delete(font);
    }
}

TEST_CASE("Font::Load Bad Argument")
{
    auto font = fontdiff::Font::Load("ddfgijhdafgkjnsdf");
    REQUIRE(font == NULL);
    delete(font);
}

TEST_CASE("Font::Load No Argument")
{
    auto font = fontdiff::Font::Load("");
    REQUIRE(font == NULL);
    delete(font);
}

TEST_CASE("Font::getWeightDistances")
{
    for(int i{0}; i < pathToFonts.size(); ++i)
    {
        auto font = fontdiff::Font::Load(pathToFonts.at(i));
        REQUIRE(font->at(0)->GetWeightDistance(0) == weightDistances1[0][i]);
        REQUIRE(font->at(0)->GetWeightDistance(14) == weightDistances1[1][i]);
        REQUIRE(font->at(0)->GetWeightDistance(9.6) == weightDistances2[i]);
        delete(font);
    }
}

TEST_CASE("Font::GetWidthDistance")
{
    for(int i{0}; i < pathToFonts.size(); ++i)
    {
        auto font = fontdiff::Font::Load(pathToFonts.at(i));
        REQUIRE(font->at(0)->GetWidthDistance(0) == 100);
        REQUIRE(font->at(0)->GetWidthDistance(12) == 88);
        REQUIRE(font->at(0)->GetWidthDistance(85.5) == 14.5);
        delete(font);
    }
}

TEST_CASE("Font::GetItalicAngle()")
{
    for(int i{0}; i < pathToFonts.size(); ++i)
    {
        auto font = fontdiff::Font::Load(pathToFonts.at(i));
        REQUIRE(font->at(0)->GetItalicAngle() == italicAngles[i]);
        delete(font);
    }
}

TEST_CASE("Font::IsCovering()")
{
    for(int i{0}; i < pathToFonts.size(); ++i)
    {
        auto font = fontdiff::Font::Load(pathToFonts.at(i));
        REQUIRE(font->at(0)->IsCovering(0) == 0);
        delete(font);
    }
}