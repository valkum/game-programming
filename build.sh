#!/bin/sh
mkdir -p build/Debug;
cd build/Debug;
cmake ../../ -DCMAKE_BUILD_TYPE=Debug;
make -j 4;
