#pragma once
#include <iostream>
#include <string>
#include <algorithm>

#include "global_enums.h"
#include "../Race.h"
#include "../Protoss.h"
#include "../Zerg.h"
#include "../Terran.h"

using namespace std;

bool comp(const pair<list<string>, int>& first, const pair<list<string>, int>& second){
    // anzahl richtiger einheiten finden und danach sortieren
    return first.second < second.second;
}

// listen bewerten
//      nach anderen kriterien als nur zeit zb
//      rush anzahl der einheiten checken
// listen sortieren
// nur die n besten listen durchlassen
//
// von Opt wird methode cutNBest aufgerufen
// 
// Vincent
class natural_selector{
    private:
    RaceType rt;
    Race evaluator;
    public:
    natural_selector(RaceType r) : rt(r){}
    natural_selector(const &natural_selector n){}
    ~natural_selector(){}

    void sortBuildLists(list<pair<list<string>, int>> &buildlists){

        buildlists->sort(comp);
    }

    void cutNBest(list<pair<list<string>, int>> &buildlists, int n){
        runBuilists(buildlists);
        buildlists->resize(n);
    }
}