#!/bin/bash
./collect.sh
cd ../collection/
./build.sh
./forwardSim.sh sc2-hots-protoss ../Protoss/advancedBuild.txt | xclip -selection c
cd ..
rm -r collection/
cd bash_scripts/
