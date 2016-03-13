#!/bin/sh
mkdir -p build/Debug;
cd build/Debug;
cmake -DCMAKE_TOOLCHAIN_FILE=~/game-programming/compile-win-mingw.cmake -DCMAKE_BUILD_TYPE=Debug -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF ../../;
make -j 8;
