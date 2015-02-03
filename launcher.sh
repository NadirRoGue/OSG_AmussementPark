#!/bin/bash

rm -r CMakeFiles
rm cmake_install.cmake
rm CMakeCache.txt
rm Makefile

cmake CMakeLists.txt
#para utilizar gdb para depurar, comentar la linea anterior y descomentar la siguiente
#cmake CMakeLists.txt -DCMAKE_BUILD_TYPE=Debug

make -f Makefile

#Ejecución normal
./sgp2
#Depuración
#gdb sgp2
