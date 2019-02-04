#!/bin/bash
./collect_opt.sh
cd ../collection
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..
cgdb --args build/Optimization/mainOptimizer rush zergling 360 1

