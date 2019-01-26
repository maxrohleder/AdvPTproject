#pragma once
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <string>

using namespace std;

typedef enum RaceType{
    PROTOSS = 1,
    ZERG = 2,
    TERRAN = 3
} RaceType;

typedef enum list_status{
    UNTESTED = -33,
    INVALID = -404,
    MAX_TIME = INT32_MAX
} lstat;

int seed = 666;

// helps to compile away analytics (writing out avg and best statistics every epoch)
const bool analytics = false;

string path_techtree_terran = "../../Optimization/techtree_terran.csv";
string path_techtree_protoss = "../../Optimization/techtree_protoss.csv";
