#!/bin/bash
./collect_opt.sh

cd ../collection
./build.sh
START=$(date +%s.%N)
./optimize.sh push dark_templar 10 > darktemplar10 &
OPTPID="$!"
sleep 1
tail -f ga_analytics.log &
wait $OPTPID || {echo "error"}
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo "total running time:"
echo $DIFF
