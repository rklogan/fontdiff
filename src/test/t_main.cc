#define CATCH_CONFIG_MAIN
#include "../third_party/catch/catch.hpp"

TEST_CASE("TEST")
{
    REQUIRE(1==1);
}

// #include "t_main.h"

// //update for each test added
// #define NUM_TESTS 3

// // incomplete test to verify the travis is working
// int main()
// {   
//     cout << "Running All Tests..." << endl;
    
//     unsigned int failed = 0;
//     failed += testArgumentParser();
//     failed += testFont();
//     failed += testFontCollection();



//     //new tests go here








//     cout << endl << endl << "TESTS COMPLETED" << endl;
//     cout << "Passed: " << NUM_TESTS - failed << endl;
//     cout << "Failed: " << failed << endl;
//     cout << "Tests Run: " << NUM_TESTS << endl;
//     assert(failed == 0);
// }