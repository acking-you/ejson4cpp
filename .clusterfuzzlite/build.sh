#!/bin/bash -eu

mkdir build
cd build
export CXXFLAGS="${CXXFLAGS} -std=gnu++17"
cmake ../
make

# Copy all fuzzer executables to $OUT/
$CXX $CXXFLAGS $LIB_FUZZING_ENGINE \
  $SRC/ejson4cpp/.clusterfuzzlite/fromjson_fuzzer.cpp \
  -o $OUT/fromjson_fuzzer \
  $SRC/ejson4cpp/build/libejson.a \
  -I$SRC/ejson4cpp/src/
