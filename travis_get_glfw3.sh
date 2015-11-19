#!/bin/sh
set -e
# check to see if protobuf folder is empty
if [ ! -d "$HOME/glfw3/lib" ]; then
  wget https://s3.eu-central-1.amazonaws.com/travis-ci-glfw3/glfw-3.1.2.tar.gz
  tar -xzvf glfw-3.1.2.tar.gz
else
  echo "Using cached directory."
fi
