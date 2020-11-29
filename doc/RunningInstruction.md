# How to Run fontdiff and Measure the Test Coverage

## Running Docker Image from Dockerhub
Due to the long build times for fontdiff, a docker image for the project and the test coverage experiment have been provided on dockerhub. These images are approximated 1.4gb in size and come with either fontdiff of the test coverage experiment and all of their dependencies pre-installed. This is by far the fastest and easiest way to run either fontdiff or the experiment.  

<details>
<summary> Running fontdiff from the Docker Container </summary>

The image can be pulled from dockerhub using:
```bash
docker pull rklogan/ecse437-fontdiff:production
```
fontdiff can then be run with it's default parameters using:
```bash
docker run -it rklogan/ecse437-fontdiff:production
./build/out/Default/fontdiff --before ./src/assets/fonts/OpenSans-Bold.ttf --after ./src/assets/fonts/OpenSans-Italic.ttf --specimen ./src/fontdiff/specimen.html --out out.pdf
```
The output pdf can then be retrieved by running the following on the host machine:
```bash
docker cp <container_name>:/fontdiff/out.pdf <host_destination>
```
In addition the test suite can be run in the docker container using the following on the host:
```bash
./build/out/Default/test
```
</details>

<details>
<summary> Running the Test Coverage Experiment from the Docker Container </summary>

The image can be pulled from dockerhub using:
```bash
docker pull rklogan/ecse437-fontdiff:coverage
```
The experiment can be run using:
```bash
docker run -it rklogan/ecse437-fontdiff:coverage
```
By default, this script will:
1. Run the test suite
2. Analyze the coverage and print a brief synopsis
3. Prompt the user to view the full report  

For more information refer to the script itself at: `/fontdiff/src/docker_run_scripts/coverage_experiment`

The full coverage report can be retrieved on the host machine using:
```bash
docker cp <container_name>:/fontdiff/coverage_report.json <host_destination>
```
</details>

---

## Building the Docker Images
The dockerfiles are provided at the root of the repository as `Dockerfile_production` and `Dockerfile_coverage`. The former can be used to run the core fontdiff software in a container. The latter can be used to run the test coverage experiment. We recommend using the provided images from dockerhub as building these images can take more than fifteen minutes. The images can be built using either of the following:
```bash
git clone --recursive https://github.com/rklogan/fontdiff
docker build -f Dockerfile_production --tag rklogan/ecse437-fontdiff:production .
docker build -f Dockerfile_coverage --tag rklogan/ecse437-fontdiff:coverage .
```
Usage is the same as was outlined in the previous sections.

---

## Building fontdiff from Scratch
<details>
    <summary> Pre-Requisites </summary>

    * g++
    * python
    * clang
    * cmake
    * libexpat1-dev
    * libz-dev
    
    On linux, these can all be fetched an installed with apt-get.
</details>

The following recipe can be followed to build the project from scratch:
```bash
git clone --recursive https://github.com/rklogan/fontdiff
cd ./fontdiff

# generate the fontdiff makefile
./src/third_party/gyp/gyp -f make --depth . --generator-output build src/fontdiff/fontdiff.gyp

# generate the coverage experiment makefile
./src/third_party/gyp/gyp -f make --depth . --generator-output coverage src/test/test_coverage.gyp

# build fontdiff
make --directory build

# build the coverage experiment
make --directory coverage
```
### Running fontdiff
fontdiff can be run using the default parameters with:
```bash
./build/out/Default/fontdiff --before ./src/assets/fonts/OpenSans-Bold.ttf --after ./src/assets/fonts/OpenSans-Italic.ttf --specimen ./src/fontdiff/specimen.html --out out.pdf
```
Which will produce `out.pdf` as output.  

### Running the Coverage Experiment
The coverage can be run using:
```bash
# run the test suite
./coverage/out/Default/test_coverage

# gather the stats into a log
cd ./coverage
gcov -n ./out/Default/obj.target/fontdiff_lib_coverage/src/fontdiff/*.o >> coverage.log

# parse the data into a json
cd ../src/test/
python generate_coverage_report.py
```
This will print overview stats to stdout and produce test_coverage.json with the detailed results in the root of the repository.



