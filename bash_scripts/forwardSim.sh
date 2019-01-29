#!/bin/bash
if [ "$#" -ne 2 ]; then
    echo "EINVAL-error! (usage: ./forwardSim.sh <race> <buildlistFile>)"
    exit 1
fi

# check if build dir exists
if [ ! -d "build" ]; then
    echo "must build programme before simulation. aborting..."
    exit 1
fi

if [ "$1" == "sc2-hots-protoss" ]; then
    build/Protoss/mainProtoss "$2" "Protoss/techtree_protoss.csv" 0
fi

if [ "$1" == "sc2-hots-terran" ]; then
    build/Terran/mainTerran "$2"
fi

if [ "$1" == "sc2-hots-zerg" ]; then
    build/Zerg/mainZerg "$2"
fi
