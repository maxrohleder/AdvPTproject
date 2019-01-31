#include <iostream>
#include <list>
#include <string>
#include <algorithm>

#include "Opt.h"
#include "global_enums.h"

using namespace std;

int main(int argc, char *argv[]){


    bool rush = false;
    string unit_to_build = "mothership";
    int amount = 1;

    Opt O(RaceType::PROTOSS, string(path_techtree_protoss), unit_to_build, amount, rush);
    O.optimize();
    O.runWinner(); 
    
    return 0;
}
