#!/bin/bash

# Script for downloading ICU4C from upstream.

pushd `dirname $0` >/dev/null
rm -rf files
curl http://download.icu-project.org/files/icu4c/57.1/icu4c-57_1-src.tgz -o icu.tgz
tar zxf icu.tgz
mv icu files
rm icu.tgz
popd >/dev/null
