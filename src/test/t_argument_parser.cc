#include "t_argument_parser.h"

TEST_CASE("Test Full Arguments")
{
    // query to check
    const char* argv[] = {"build/out/Default/fontdiff",
                          "--before", "path/to/old/FooFont*.ttf",
                          "--after", "path/to/new/FooFont*.ttf",
                          "--specimen", "src/fontdiff/specimen.html",
                          "--out", "out.pdf"};
    fontdiff::ArgumentParser ap = fontdiff::ArgumentParser(9, argv);

    auto before = vector<string>({"path/to/old/FooFont*.ttf"});
    REQUIRE(compareVectors(ap.beforeFonts(), before));

    auto after = vector<string>({"path/to/new/FooFont*.ttf"});
    REQUIRE(compareVectors(ap.afterFonts(), after));

    auto testSpec = string("src/fontdiff/specimen.html");
    REQUIRE(testSpec.compare(ap.specimen()) == 0);

    auto testOut = string("out.pdf");
    REQUIRE(testOut.compare(ap.output()) == 0);
}

TEST_CASE("Test Multiple Before Arguments")
{
    // query to check
    const char* argv[] = {"build/out/Default/fontdiff",
                          "--before", "path/to/old/FooFont*.ttf",
                          "a/b/d.ttf", 
                          "--after", "path/to/new/FooFont*.ttf",
                          "--specimen", "src/fontdiff/specimen.html",
                          "--out", "out.pdf"};
    fontdiff::ArgumentParser ap = fontdiff::ArgumentParser(10, argv);

    // check that the before values were parsed correctly
    // then check each of the other there fields
    auto before = vector<string>({"path/to/old/FooFont*.ttf", 
                          "a/b/d.ttf"});
    REQUIRE(compareVectors(ap.beforeFonts(), before));

    auto after = vector<string>({"path/to/new/FooFont*.ttf"});
    REQUIRE(compareVectors(ap.afterFonts(), after));

    auto testSpec = string("src/fontdiff/specimen.html");
    REQUIRE(testSpec.compare(ap.specimen()) == 0);

    auto testOut = string("out.pdf");
    REQUIRE(testOut.compare(ap.output()) == 0);
}


TEST_CASE("Test Multiple After Arguments")
{
    // query to check
    const char* argv[] = {"build/out/Default/fontdiff",
                          "--before", "path/to/old/FooFont*.ttf",
                          "--after", "path/to/new/FooFont*.ttf", "dsfg/ytukj/sdf/dfg.ttf",
                          "--specimen", "src/fontdiff/specimen.html",
                          "--out", "out.pdf"};
    fontdiff::ArgumentParser ap = fontdiff::ArgumentParser(10, argv);

    // check that the before values were parsed correctly
    // then check each of the other there fields
    auto before = vector<string>({"path/to/old/FooFont*.ttf"});
    REQUIRE(compareVectors(ap.beforeFonts(), before));

    auto after = vector<string>({"path/to/new/FooFont*.ttf", "dsfg/ytukj/sdf/dfg.ttf"});
    REQUIRE(compareVectors(ap.afterFonts(), after));

    auto testSpec = string("src/fontdiff/specimen.html");
    REQUIRE(testSpec.compare(ap.specimen()) == 0);

    auto testOut = string("out.pdf");
    REQUIRE(testOut.compare(ap.output()) == 0);
}

TEST_CASE("Test Multiple Before and Multiple After Arguments")
{
    // query to check
    const char* argv[] = {"build/out/Default/fontdiff",
                          "--before", "path/to/old/FooFont*.ttf",
                          "a/b/d.ttf", 
                          "--after", "path/to/new/FooFont*.ttf", "dsfg/ytukj/sdf/dfg.ttf",
                          "--specimen", "src/fontdiff/specimen.html",
                          "--out", "out.pdf"};
    fontdiff::ArgumentParser ap = fontdiff::ArgumentParser(11, argv);

    // check that the before values were parsed correctly
    // then check each of the other there fields
    auto before = vector<string>({"path/to/old/FooFont*.ttf", 
                          "a/b/d.ttf"});
    REQUIRE(compareVectors(ap.beforeFonts(), before));

    auto after = vector<string>({"path/to/new/FooFont*.ttf", "dsfg/ytukj/sdf/dfg.ttf"});
    REQUIRE(compareVectors(ap.afterFonts(), after));

    auto testSpec = string("src/fontdiff/specimen.html");
    REQUIRE(testSpec.compare(ap.specimen()) == 0);

    auto testOut = string("out.pdf");
    REQUIRE(testOut.compare(ap.output()) == 0);
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