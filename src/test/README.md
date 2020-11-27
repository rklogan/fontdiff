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
2. Create you source code
   1. Name: ```./src/test/t_<module_to_test>.cc```
   2. Include your header:
   ```c++
   #include "t_<module_to_test>.h"
   ```
   3. Implement your tests in this file. See documetation for `catch` for details.
3. Add your tests the the test suite header ```./src/test/t_main.h```
   * See the comment in the code for there to put the following line:
   ```c++
   #include "t_<module_to_test>.cc"

## Development
1. Generate build instructions (only needed first time or if drastic changes are implemented):
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

## Building for Measuring Test Coverage
1. Configure the test coverage build
```bash
./src/third_party/gyp/gyp -f make --depth . --generator-output coverage src/test/test_coverage.gyp
```
2. Make the test coverage build
```bash
make --directory coverage
```
3. Run the coverage build. This will generate the raw coverage data.
```bash
./coverage/out/Default/test_coverage
```
4. Generate readable data. Omit the `-n` flag to get line by line data in `./coverage/out/Default/obj.target/fontdiff_lib_coverage/src/fontdiff/`. 
```bash
cd ./coverage
gcov -n ./out/Default/obj.target/fontdiff_lib_coverage/src/fontdiff/*.o >> coverage.log
```
5. Generate a `coverage_report.json` in the root of the repository.
```bash
cd ../src/test/
python generate_coverage_report.py
```

## Notes
1. fontdiff's directory structure relies on gyp to link things correctly. If you try to compile a test that includes modules from fontdiff directly it will likely fail; the include statements in fontdiff make many assumptions about where the CWD and many other files are.
2. Many of the dependencies are linked using the build tool, gyp. This can result in code editors/IDEs highlighting #include statements as being erroneous. You can either ignore the red squiggles, and build as denoted above, or setup your editor/IDE to integrate gyp. I have not tried the latter, but it should be possible.

   
