#!/bin/sh
mkdir -p build/Release;
cd build/Release;
cmake -DCMAKE_BUILD_TYPE=Release -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF ../../;
make -j 4;
