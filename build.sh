#!/bin/sh
mkdir -p build/Debug;
cd build/Debug;
cmake -DCMAKE_TOOLCHAIN_FILE=~/game-programming/compile-win-mingw.cmake -DCMAKE_BUILD_TYPE=Debug ../../;
make -j 8;
