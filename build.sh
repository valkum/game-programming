#!/bin/sh

mkdir -p build/Debug;
cd build/debug;
cmake ../../ -DCMAKE_BUILD_TYPE=Debug;
make;
