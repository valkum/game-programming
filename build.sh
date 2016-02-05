#!/bin/sh
mkdir -p build/Debug;
cd build/Debug;
cmake -DCMAKE_BUILD_TYPE=Debug -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF ../../;
make -j 8;
