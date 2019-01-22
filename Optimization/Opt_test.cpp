#include <string>
#include "Opt.h"
#include <iostream>

int main(){
    Opt o("../../Optimization/techtree_zerg.csv", "mutalisk", 1, false, 'z');
    o.runPureRandomDebug();
    o.printBuildlists();
    return 0;
}