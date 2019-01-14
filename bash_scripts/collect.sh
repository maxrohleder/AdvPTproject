#!/bin/bash
cd ..
if [ ! -d "collection" ]; then
    mkdir collection
else
    rm -r collection
    mkdir collection
fi

#dirs for races
mkdir collection/Protoss
mkdir collection/Validator
mkdir collection/Validator/validator_by_file
mkdir collection/Terran
mkdir collection/Zerg

#copy over all scripts
cp CMakeLists.txt collection/
cp Race.h collection/

#Protoss
cp Protoss.h collection/
cp Protoss/CMakeLists.txt collection/Protoss/
cp Protoss/mainProtoss.cpp collection/Protoss/
cp Protoss/Protoss_status.h collection/Protoss/
cp Validator/validator_by_file/parser.h collection/Validator/validator_by_file/
cp Validator/validator_by_file/techtree_protoss.csv collection/Validator/validator_by_file/

#Terran
cp Terran.h collection/
cp Terran_header.h collection/
cp Terran/CMakeLists.txt collection/Terran/
cp Terran/mainTerran.cpp collection/Terran/

#Zerg
cp Zerg.h collection/
cp Zerg_header.h collection/
cp Zerg/CMakeLists.txt collection/Zerg/
cp Zerg/mainZerg.cpp collection/Zerg/

#copy bash scrpits
cp bash_scripts/build.sh collection/
cp bash_scripts/forwardSim.sh collection/

cd bash_scripts

