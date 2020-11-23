#include <string>
#include <vector>
#include <iostream>

#include "../fontdiff/argument_parser.h"
using namespace std;

int testArgumentParser();
bool testFullArguments();
bool testMultiBefore();
bool testMultiAfter();
bool testMultiBeforeMultiAfter();
bool compareVectors(vector<string> v1, vector<string> v2);

