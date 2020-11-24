#include "t_font_collection.h"

/**
 * Runs all the tests for the font_collection class
 * Coverage 92/92 lines = 100% coverage
 * @return 1 if the test failed; 0 otherwise
 */
int testFontCollection()
{
    cout << "Testing Module: font_collection" << endl;
    bool passed = testEmptyCollection();
    passed &= testSingleItemCollection();
    passed &= testMultiItemCollection();
    
    if(passed)
    {
        cout << "\t\tTest Passed" << endl;
        return 0;
    }
    cout << "\t\t Test Failed" << endl;
    return 1;
}

/**
 * Subtest for empty collections
 * @return true if the test passed; false otherwise
 */
bool testEmptyCollection()
{
    vector<const fontdiff::Font*> fonts = vector<const fontdiff::Font*>();
    fontdiff::FontCollection fc = fontdiff::FontCollection(fonts);

    const string spec = "";
    fontdiff::Style* style = new fontdiff::Style(NULL, NULL, spec);
    auto font = fontdiff::Font::Load("./src/assets/fonts/OpenSans-BoldItalic.ttf")->at(0);

    bool passed = fc.FindFont(0, NULL, NULL) == 0;
    passed &= fc.FindFont(0, style, NULL) == 0;
    passed &= fc.FindFont(0, NULL, font) == 0;
    passed &= fc.FindFont(0, style, font) == 0;

    delete(style); delete(font);

    if(!passed)
        cout << "\t\tTest Failed for empty font_collections" << endl;
    return passed;
}

/**
 * Subtest for collections with a single item
 * @return true if the test passed; false otherwise
 */
bool testSingleItemCollection()
{
    vector<const fontdiff::Font*> fonts = vector<const fontdiff::Font*>();
    auto font = fontdiff::Font::Load("./src/assets/fonts/OpenSans-BoldItalic.ttf");
    fonts.push_back(font->at(0));
    fontdiff::FontCollection fc = fontdiff::FontCollection(fonts);

    const string spec = "";
    fontdiff::Style* style = new fontdiff::Style(NULL, NULL, spec);

    bool passed = fc.FindFont(0, NULL, NULL) == 0;
    passed &= fc.FindFont(0, style, NULL) == 0;
    passed &= fc.FindFont(0, NULL, font->at(0)) == 0;
    passed &= fc.FindFont(100, NULL, font->at(0)) != 0;
    passed &= fc.FindFont(0, style, font->at(0)) == 0;
    passed &= fc.FindFont(100, style, font->at(0)) != 0;
    
    delete(style); delete (font);

    if(!passed)
        cout << "\t\tTest Failed for font_collections with a single entry" << endl;

    return passed;
}

/**
 * Subtest for collections with more than one item
 * @return true if the test passed; false otehrwise
 */
bool testMultiItemCollection()
{
    vector<const fontdiff::Font*> fonts = vector<const fontdiff::Font*>();
    auto font1 = fontdiff::Font::Load("./src/assets/fonts/OpenSans-BoldItalic.ttf");
    auto font2 = fontdiff::Font::Load("./src/assets/fonts/OpenSans-Light.ttf");
    fonts.push_back(font1->at(0)); 
    fonts.push_back(font2->at(0));
    fontdiff::FontCollection fc = fontdiff::FontCollection(fonts);

    const string spec = "";
    fontdiff::Style* style = new fontdiff::Style(NULL, NULL, spec);

    bool passed = fc.FindFont(0, NULL, NULL) == 0;
    passed &=  fc.FindFont(0, style, NULL) == 0;
    passed &= fc.FindFont(0, NULL, font1->at(0)) == 0;
    passed &= fc.FindFont(0, NULL, font2->at(0)) == 0;
    passed &= (fc.FindFont(100, NULL, font1->at(0)) == font1->at(0));
    passed &= (fc.FindFont(100, NULL, font2->at(0)) == font2->at(0));
    passed &= fc.FindFont(0, style, font1->at(0)) == 0;
    passed &= (fc.FindFont(100, style, font1->at(0)) == font1->at(0));

    delete(font1); delete(font2); delete(style);

    if(!passed)
        cout << "\t\tTest Failed for font_collections with multiple entries" << endl;

    return passed;
}