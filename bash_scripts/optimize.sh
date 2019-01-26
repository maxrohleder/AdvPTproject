#!/bin/bash
if [ "$#" -ne 3 ]; then
    echo "EINVAL-error! (usage: ./optimize.sh  <rush/push> <unit> <seconds/amount>)"
    exit 1
fi

# check if build dir exists
if [ ! -d "build" ]; then
    echo "must build programme before optimization. aborting..."
    exit 1
fi

build/Optimization/mainOptimizer "$1" "$2" "$3"