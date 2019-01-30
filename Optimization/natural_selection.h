#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <list>

#include "global_enums.h"
#include "../Race.h"
#include "../Protoss.h"
#include "../Zerg.h"
#include "../Terran.h"

using namespace std;

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
    //natural_selector(const &natural_selector n){}
    ~natural_selector(){}

    void sortBuildLists(list<pair<list<string>, int>> &buildlists, string rushedUnit){
        for (auto const& i : buildlists) {
            buildlists.sort(comp);
        }
        
    }

    void cutNBest(list<pair<list<string>, int>> &buildlists, int n){
        //runBuilists(buildlists);
        //buildlists->resize(n);

        list<pair<list<string>, int>>::iterator position_n, position_end;
        position_n = position_end = buildlists.begin();
        advance(position_n, n);
        advance(position_end, buildlists.end());
        buildlists.erase(position_n, position_end);
    }
};