
#include "t_main.h"

// incomplete test to verify the travis is working
int main()
{   
    printf("Running All Tests...\n");
    
    unsigned int failed = 0;
    failed += testArgumentParser();












    const unsigned int num_tests = 1;

    printf("\n\n\n%s\n", "TESTS COMPLETED");
    printf("%s%d\n", "Passed: ", num_tests - failed);
    printf("%s%d\n", "Failed: ", failed);
    printf("%s%d\n", "Tests Run: ", num_tests);
    assert(failed == 0);
}