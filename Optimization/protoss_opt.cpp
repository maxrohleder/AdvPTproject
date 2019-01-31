#include <iostream>
#include <list>
#include <string>
#include <algorithm>

#include "Opt.h"
#include "global_enums.h"

using namespace std;

int main(int argc, char *argv[]){


    bool rush = false;
    string unit_to_build = "dark_templar";
    int amount = 1;

    path_techtree_protoss = "../Protoss/techtree_protoss.csv";

    Opt O(RaceType::PROTOSS, path_techtree_protoss, unit_to_build, amount, rush);
    O.optimize();
    O.runWinner(); 
    
    return 0;
}
