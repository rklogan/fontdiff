#include "t_argument_parser.h"

/**
 * Runs all the tests for the argument parser class
 * Covers regression on 107/132 lines = 81%
 * @return 1 if the test failed; 0 otherwise
 */
int testArgumentParser()
{
    cout << "Testing Module: argument_parser..." << endl;
    bool passed = testFullArguments();
    passed &= testMultiBefore();
    passed &= testMultiAfter();
    passed &= testMultiBeforeMultiAfter();

    if(passed){
        cout << "\t\tTest Passed" << endl;
        return 0;
    }
    cout << "\t\tTest Failed" << endl;
    return 1;
}

/**
 * Subtest to check the default parameters
 * @return true if the test was passed; false otherwise
 */
bool testFullArguments()
{
    // query to check
    const char* argv[] = {"build/out/Default/fontdiff",
                          "--before", "path/to/old/FooFont*.ttf",
                          "--after", "path/to/new/FooFont*.ttf",
                          "--specimen", "src/fontdiff/specimen.html",
                          "--out", "out.pdf"};
    fontdiff::ArgumentParser ap = fontdiff::ArgumentParser(9, argv);

    bool rv;

    // check that the before values were parsed correctly
    // then check each of the other there fields
    vector<string> before = vector<string>({"path/to/old/FooFont*.ttf"});
    rv = compareVectors(ap.beforeFonts(), before);

    vector<string> after = vector<string>({"path/to/new/FooFont*.ttf"});
    rv &= compareVectors(ap.afterFonts(), after);

    string testSpec = string("src/fontdiff/specimen.html");
    rv &= (testSpec.compare(ap.specimen()) == 0);

    string testOut = string("out.pdf");
    rv &= (testOut.compare(ap.output()) == 0);
    
    if(!rv)
        cout << "\targument_parser failed a test for the default arguments." << endl;
    return rv;
}

/**
 * Subtest for multiple before arguments
 * @return true if the test was passed; false otherwise
 */
bool testMultiBefore()
{
    // query to check
    const char* argv[] = {"build/out/Default/fontdiff",
                          "--before", "path/to/old/FooFont*.ttf",
                          "a/b/d.ttf", 
                          "--after", "path/to/new/FooFont*.ttf",
                          "--specimen", "src/fontdiff/specimen.html",
                          "--out", "out.pdf"};
    fontdiff::ArgumentParser ap = fontdiff::ArgumentParser(10, argv);

    bool rv;

    // check that the before values were parsed correctly
    // then check each of the other there fields
    vector<string> before = vector<string>({"path/to/old/FooFont*.ttf", 
                          "a/b/d.ttf"});
    rv = compareVectors(ap.beforeFonts(), before);

    vector<string> after = vector<string>({"path/to/new/FooFont*.ttf"});
    rv &= compareVectors(ap.afterFonts(), after);

    string testSpec = string("src/fontdiff/specimen.html");
    rv &= (testSpec.compare(ap.specimen()) == 0);

    string testOut = string("out.pdf");
    rv &= (testOut.compare(ap.output()) == 0);
    
    if(!rv)
        cout << "\t argument_parser failed a test with multiple before arguments" << endl;
    return rv;
}

/**
 * Subtest to check multiple after parameters
 * @return true if the test was passed; false otherwise
 */
bool testMultiAfter()
{
    // query to check
    const char* argv[] = {"build/out/Default/fontdiff",
                          "--before", "path/to/old/FooFont*.ttf",
                          "--after", "path/to/new/FooFont*.ttf", "dsfg/ytukj/sdf/dfg.ttf",
                          "--specimen", "src/fontdiff/specimen.html",
                          "--out", "out.pdf"};
    fontdiff::ArgumentParser ap = fontdiff::ArgumentParser(10, argv);

    bool rv;

    // check that the before values were parsed correctly
    // then check each of the other there fields
    vector<string> before = vector<string>({"path/to/old/FooFont*.ttf"});
    rv = compareVectors(ap.beforeFonts(), before);

    vector<string> after = vector<string>({"path/to/new/FooFont*.ttf", "dsfg/ytukj/sdf/dfg.ttf"});
    rv &= compareVectors(ap.afterFonts(), after);

    string testSpec = string("src/fontdiff/specimen.html");
    rv &= (testSpec.compare(ap.specimen()) == 0);

    string testOut = string("out.pdf");
    rv &= (testOut.compare(ap.output()) == 0);
    
    if(!rv)
        cout << "\t argument_parser failed a test for multiple after arguments" << endl;
    return rv;
}

/**
 * Subtest for multiple before and after arguments
 * @return true if the test was passed; false otherwise
 */
bool testMultiBeforeMultiAfter()
{
    // query to check
    const char* argv[] = {"build/out/Default/fontdiff",
                          "--before", "path/to/old/FooFont*.ttf",
                          "a/b/d.ttf", 
                          "--after", "path/to/new/FooFont*.ttf", "dsfg/ytukj/sdf/dfg.ttf",
                          "--specimen", "src/fontdiff/specimen.html",
                          "--out", "out.pdf"};
    fontdiff::ArgumentParser ap = fontdiff::ArgumentParser(11, argv);

    bool rv;

    // check that the before values were parsed correctly
    // then check each of the other there fields
    vector<string> before = vector<string>({"path/to/old/FooFont*.ttf", 
                          "a/b/d.ttf"});
    rv = compareVectors(ap.beforeFonts(), before);

    vector<string> after = vector<string>({"path/to/new/FooFont*.ttf", "dsfg/ytukj/sdf/dfg.ttf"});
    rv &= compareVectors(ap.afterFonts(), after);

    string testSpec = string("src/fontdiff/specimen.html");
    rv &= (testSpec.compare(ap.specimen()) == 0);

    string testOut = string("out.pdf");
    rv &= (testOut.compare(ap.output()) == 0);
    
    if(!rv)
        cout << "\targument_parser failed a test with multiple before and after arguments" << endl;
    return rv;
}

/**
 * Utility to compared to string vectors
 * @param v1 A vector of strings to be compared
 * @param v2 A vector of strings to be compared 
 * @return True iff every string in the vector is the same
 */
bool compareVectors(vector<string> v1, vector<string> v2)
{
    int size = v1.size();
    if(size != v2.size()) return false;

    for(int i{0}; i < size; ++i)
    {
        if(v1[i] != v2[i])
            return false;
    }
    return true;
}