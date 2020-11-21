#!/bin/bash

for test in ./src/test/*.cc; do
    clang "$test";
    ./a.out
done
