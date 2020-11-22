
#include "t_main.h"

#define NUM_TESTS 1

// incomplete test to verify the travis is working
int main()
{   
    printf("Running All Tests...\n");
    
    unsigned int failed = 0;
    failed += testArgumentParser();













    printf("\n\n\n%s\n", "TESTS COMPLETED");
    printf("%s%d\n", "Passed: ", NUM_TESTS - failed);
    printf("%s%d\n", "Failed: ", failed);
    printf("%s%d\n", "Tests Run: ", NUM_TESTS);
    assert(failed == 0);
}