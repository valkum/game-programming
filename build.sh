#!/bin/sh

mkdir build/Debug;
cd build/debug;
cmake ../../ -DCMAKE_BUILD_TYPE=Debug;
make;
