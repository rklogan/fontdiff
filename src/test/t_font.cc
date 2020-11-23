#include "t_font.h"

/**
 * Runs all the tests for the Font class
 * Covers 341/454 lines = 75%
 * @return 1 if the test failed; 0 otherwise
 */
int testFont()
{
    cout << "Testing Module: font" << endl;

    vector<string> pathToFonts = vector<string>({
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

    bool passed = testLoad(pathToFonts);
    passed &= testOther(pathToFonts);

    if(passed){
        cout << "\t\tTest Passed" << endl;
        return 0;
    }
    cout << "\t\t Test Failed" << endl;
    return 1;
}
/**
 * Subtest for the Load function
 * @return true if the test passed; false otherwise
 */
bool testLoad(vector<string> pathToFonts)
{

    for(auto it = pathToFonts.begin(); it != pathToFonts.end(); ++it)
    {
       auto font = fontdiff::Font::Load(*it);

        if(font == NULL)
        {
            cout << "\tTest Failed: Singular Font Loading Failed" << endl;
            return false;
        }
        delete(font);
    }

    auto font = fontdiff::Font::Load("adfggo;iuhasdfgkhuj");
    if(font != NULL)
    {
        cout << "\tTest Failed: Loading a nonexistant font didn't load NULL" << endl;
        delete(font);
        return false;
    }

    font = fontdiff::Font::Load("");
    if(font != NULL)
    {
        cout << "\tTest Failed: A font was loaded when no path was provided" << endl;
        delete(font);
        return false;
    }

    return true;
}

/**
 * Subtest for all directly verifiable functions
 */
bool testOther(vector<string> pathToFonts)
{
    // Test Solutions
    int weightDistances1[2][11] = {
        {700, 700, 800, 800, 400, 300, 300, 400, 600, 600, 700},
        {686, 686, 786, 786, 386, 286, 286, 386, 586, 586, 686}
    };

    double weightDistances2[11] 
        = {690.4, 690.4, 790.4, 790.4, 390.4, 290.4, 290.4, 390.4, 590.4, 590.4, 690.4};

    int italicAngles[11] =
        {-786432, 0, 0, -786432, -786432, 0, -786432, 0, 0, -786432, -786432};

    // begin testing
    for(int i{0}; i < pathToFonts.size(); ++i)
    {
        auto font = fontdiff::Font::Load(pathToFonts.at(i));

        // TEST the getWeightDistance function
        if ( font->at(0)->GetWeightDistance(0) != weightDistances1[0][i]
            || font->at(0)->GetWeightDistance(14) != weightDistances1[1][i]
            || font->at(0)->GetWeightDistance(9.6) != weightDistances2[i] )
        {
            cout << "\tTest Failed: GetWeightDistance produced erroneous results" << endl;
            delete(font);
            return false;
        }

        // TEST the getWidthDistance fucntion
        if ( font->at(0)->GetWidthDistance(0) != 100
            || font->at(0)->GetWidthDistance(12) != 88
            || font->at(0)->GetWidthDistance(85.5) != 14.5)
        {
            cout << "\tTest Failed: GetWidthDistance produced erroneous results" << endl;
            delete(font);
            return false;
        }

        // TEST getItalicAngle
        if(font->at(0)->GetItalicAngle() != italicAngles[i])
        {
            cout << "\tTest Failed: GetItalicAngle produced erroneous results" << endl;
            delete(font);
            return false;
        }

        // TEST IsCovering
        if(font->at(0)->IsCovering(0) != 0)
        {
            cout << "\tTest Failed: IsCovering produced erroneous results" << endl;
            delete(font);
            return false;
        }

    }
    return true;
}