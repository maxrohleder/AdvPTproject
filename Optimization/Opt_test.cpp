#include <string>
#include "Opt.h"
#include <iostream>
#include "global_enums.h"

int main(){
    Opt o(RaceType::ZERG, "../../Optimization/techtree_zerg.csv", "roach", 20, false);
    o.runPureRandomDebug(true);
    return 0;
}