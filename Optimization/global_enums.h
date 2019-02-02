#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include <list>
#include <algorithm>
#include <vector>

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
const bool analytics = true;

string path_techtree_terran = "../../Optimization/techtree_terran.csv";
string path_techtree_protoss = "../../Optimization/techtree_protoss.csv";
string path_techtree_zerg = "../../Optimization/techtree_zerg.csv";

// who added param rushedUnit? makes no sense in comp of two pairs
bool comp(const pair<list<string>, int>& first, const pair<list<string>, int>& second){
    // anzahl richtiger einheiten finden und danach sortieren
    return first.second < second.second;
}
