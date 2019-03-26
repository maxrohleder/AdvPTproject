#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include <list>
#include <algorithm>
#include <vector>

using namespace std;

typedef enum RaceType
{
    PROTOSS = 1,
    ZERG = 2,
    TERRAN = 3
} RaceType;

typedef enum list_status
{
    UNTESTED = -33,
    INVALID = -404,
    MAX_TIME = INT32_MAX
} lstat;

int rushpush_max_time = 5000;

int seed = 1; //9242

// helps to compile away analytics (writing out avg and best statistics every epoch)
bool analytics = true;

string path_techtree_terran = "../../Optimization/techtree_terran.csv";
string path_techtree_protoss = "../../Optimization/techtree_protoss.csv";
string path_techtree_zerg = "../../Optimization/techtree_zerg.csv";

bool comp(const pair<list<string>, int> &first, const pair<list<string>, int> &second)
{
    // anzahl richtiger einheiten finden und danach sortieren
    return first.second < second.second;
}

bool rush_compare(const pair<list<string>, int> &list1, const pair<list<string>, int> &list2)
{
    // je negativer eine liste ist desto besser (abs(list1.second) == no_targets) as defined in natural selection
    return list1.second < list2.second;
}

bool compWithSize(const pair<list<string>, int> &list1, const pair<list<string>, int> & list2)
{
    if(list1.second == list2.second){
        auto it1 = list1.first.begin();
        auto it2 = list2.first.begin();
        if(list1.first.size() != list2.first.size()){
            return list1.first.size() < list2.first.size();
        }
        for(int i = 0; i < list1.first.size(); ++i, ++it1, ++it2){
            if(*it1 != *it2){
                return *it1 < *it2;
            }
        }
        return true;
    }else
    {
        return list1.second < list2.second;
    }
    
}
