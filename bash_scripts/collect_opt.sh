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
mkdir collection/Terran
mkdir collection/Zerg
mkdir collection/Optimization

#copy over all scripts
cp CMakeLists.txt collection/
cp Race.h collection/

#Protoss
cp Protoss.h collection/
cp Protoss/CMakeLists.txt collection/Protoss/
cp Protoss/mainProtoss.cpp collection/Protoss/
cp Protoss/Protoss_status.h collection/Protoss/
cp Protoss/parser_protoss.h collection/Protoss/
cp Protoss/techtree_protoss.csv collection/Protoss/

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

#Optimization
cp Optimization/CMakeLists.txt collection/Optimization
cp Optimization/global_enums.h collection/Optimization
cp Optimization/list_builder.h collection/Optimization
cp Optimization/mutations.h collection/Optimization
cp Optimization/natural_selection.h collection/Optimization
cp Optimization/Opt.h collection/Optimization
cp Optimization/parser.h collection/Optimization
cp Optimization/techtree* collection/Optimization
cp Optimization/mainOptimizer.cpp collection/Optimization

#Validator
cp Validator/Validator.h collection/Validator
cp Validator/ValidatorZerg.h collection/Validator
#cp Validator/ValidatorTerran.h collection/Validator


#copy bash scrpits
cp bash_scripts/build.sh collection/
cp bash_scripts/forwardSim.sh collection/
cp bash_scripts/optimize.sh collection/
 
cd bash_scripts
