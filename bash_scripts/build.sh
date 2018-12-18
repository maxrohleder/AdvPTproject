#!/bin/bash
#TODO change to unix
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
cmake ..
make
cd ..