if [ "$#" -ne 2 ]; then
    echo "illegal number of arguments"
fi

if [ "$1" == "sc2-hots-protoss" ]; then
    build/Protoss/mainProtoss "$2"
fi
if [ "$1" == "sc2-hots-zerg" ]; then
    build/Zerg/mainZerg "$2"
fi
if [ "$1" == "sc2-hots-terran" ]; then
    build/Terran/mainTerran "$2"
fi