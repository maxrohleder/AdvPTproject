#include <string>
#include "Opt.h"
#include <iostream>
#include "global_enums.h"

int main(){
    //Opt o(RaceType::ZERG, "../../Optimization/techtree_zerg.csv", "spire", 3, false);
    //Opt o(RaceType::TERRAN, "../../Optimization/techtree_terran.csv", "battlecruiser", 1, false);
    Opt o(RaceType::PROTOSS, "../../Optimization/techtree_protoss.csv", "mothership", 1, false);
    o.runPureRandomDebug(true);
    return 0;
}