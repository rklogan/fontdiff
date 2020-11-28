FROM ubuntu:latest AS base

# Install dependencies
RUN apt-get update
RUN apt-get -y install g++
RUN apt-get -y install python
RUN apt-get -y install clang
RUN apt-get -y install cmake
RUN apt-get -y install libexpat1-dev
RUN apt-get -y install libz-dev

FROM base as coverage

COPY ./src ./fontdiff/src
WORKDIR /fontdiff

# Build the project
RUN ./src/third_party/gyp/gyp -f make --depth . --generator-output coverage src/test/test_coverage.gyp
RUN make --directory coverage

# CMD ./src/test/generate_coverage_report.py
CMD /fontdiff/src/coverage_experiment/coverage_experiment