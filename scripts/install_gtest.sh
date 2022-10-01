#!/bin/bash
version=1.12.1

if [ ! -e "/usr/local/lib/libgtest.a" ]; then
  git clone https://github.com/google/googletest.git -b release-${version}
  cd googletest
  mkdir build
  cd build
  cmake ..
  make
  sudo make install
  cd ../../
  rm -rf googletest
fi
