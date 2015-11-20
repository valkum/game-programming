#!/bin/sh
set -e
# check to see if protobuf folder is empty
if [ ! -d "$HOME/glfw3/lib" ]; then
  cd $HOME
  wget https://s3.eu-central-1.amazonaws.com/travis-ci-glfw3/glfw-3.1.2.tar.gz
  tar -xzvf glfw-3.1.2.tar.gz
  cp $HOME/glfw3/include/ /usr/local/include
  cp $HOME/glfw3/lib/ /usr/local/lib
else
  echo "Using cached directory."
  cp -r $HOME/glfw3/include/* /usr/include/
  cp -r $HOME/glfw3/lib/* /usr/lib/
fi

