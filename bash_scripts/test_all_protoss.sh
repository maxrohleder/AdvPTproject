#!/bin/bash
./collect_opt.sh

cd ../collection
./build.sh
START=$(date +%s.%N)
./optimize.sh push colossus 3 > colossus3p &
OPTPID="$!"
sleep 1
START2=$(date +%s.%N)
./optimize.sh push void_ray 1 > voidray1p
VOIPID="$!"
sleep 1
START3=$(date +%s.%N)
./optimize.sh rush stalker 600 > stalker600r
STALPID="$!"
sleep 1
START4=$(date +%s.%N)
./optimize.sh rush zealot 360 > zealot360r
ZEAPID=$(date +%s.%N)
sleep 1
tail -f ga_analytics.log &
wait $OPTPID || {echo "error"}
wait $VOIPID
wait $STALPID
wait $ZEAPID
END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo "total running time:"
echo $DIFF
