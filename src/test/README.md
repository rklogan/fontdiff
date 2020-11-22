# Testing
## How to add new test modules
1. Create a header
   1. Name: ```./src/test/t_<module_to_test>.h```
   2. Include anything you need from the STL
   3. Include the module to test:  
   ```c++
   #include "../fontdiff/<module_to_test>.h
   ```
   4. Optionally:  
   ```c++
   using namespace std;
   ```
   5. Write templates for all the functions that will be written (you can add to these later, as needed)
2. Create you source code
   1. Name: ```./src/test/t_<module_to_test>.cc```
   2. Include your header:
   ```c++
   #include "t_<module_to_test>.h"
   ```
   3. Implement your tests in this file
      * To keep things simple have your tests return 0 if they passed, 1 if they failed.
3. Add your tests the the test suite header ```./src/test/t_main.h```
   * See the comment in the code for there to put the following line:
   ```c++
   #include "t_<module_to_test>.cc"
4. Update The test suite
   1. Add the number of tests you are adding to the value stored on line 5. 
   For instance, if this was 1 and I'm adding 4 tests, I should change this to be 5.
   ```c++
   #define NUM_TESTS 1
   ```
   2. For each test you add a line:
   ```c++
   failed += nameOfTest();
   ```
   where indicated in the comments (~line 13).

## Development
1. Generate build instructions (only needed when files are added):
```bash
./src/third_party/gyp/gyp -f make --depth . --generator-output build src/fontdiff/fontdiff.gyp
```
2. Build
```bash
make --directory build
```
3. To run the test suite, you can run
```bash
./build/out/Default/test
```

## Notes
fontdiff's directory structure relies on gyp to link things correctly. If you try to compile a test that includes modules from fontdiff directly it will likely fail; the include statements in fontdiff make many assumptions about where the CWD and many other files are.

   
