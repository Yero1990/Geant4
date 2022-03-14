#!/bin/sh

#compile geant4 example
rm -rf CMakeFiles cmake_install.cmake Makefile CMakeCache.txt
make clean
cmake -DCMAKE_INSTALL_PREFIX=~/geant4-source/geant4-v11.0.1-install/lib/Geant4-11.0.1/ ..
make -j4
make install



