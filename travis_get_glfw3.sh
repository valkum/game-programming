#!/bin/sh
set -e
# check to see if protobuf folder is empty
if [ ! -d "$HOME/glfw3/lib" ]; then
  wget https://github.com/glfw/glfw/archive/3.1.1.tar.gz
  tar -xzvf 3.1.1.tar.gz
  cd glfw-3.1.1 && cmake -DCMAKE_INSTALL_PREFIX=$HOME/glfw3 -DBUILD_SHARED_LIBS=ON . && make && make install
else
  echo "Using cached directory."
fi
