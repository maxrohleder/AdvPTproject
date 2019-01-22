#include <string>
#include "Opt.h"
#include <iostream>

int main(){
    Opt o("../../Optimization/techtree_zerg.csv", "roach", 10, false, 'z');
    o.runPureRandomDebug();
    o.printBuildlists();
    return 0;
}