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
    bool rush;
    string target;
    Race evaluator;
    public:
    natural_selector(RaceType r, bool rush, string target) : rt(r), rush(rush), target(target) {}
    //natural_selector(const &natural_selector n){}
    ~natural_selector(){}

    void sortBuildLists(list<pair<list<string>, int>> &buildlists, string rushedUnit){
        for (auto const& i : buildlists) {
            buildlists.sort(comp);
        }
        
    }

    void medianCut(list<pair<list<string>, int>> &buildlists){
        // TODO calculate median and cut all lists above. (ignore all INT_MAX times) (performes well on outliers)
    }

    void cutNBest(list<pair<list<string>, int>> &buildlists, int n, bool median_cut = false){
        if(rush){
            // overwrites second integer with number of targets * -1 (to be able to differenciate between time and amount)
            sort_wrt_amout(buildlists);
        }
        if(median_cut){
            medianCut(buildlists);
        }else {
            buildlists.resize(n);
        }
    }

    void sort_wrt_amout(list<pair<list<string>, int>> &buildlists){
        for(pair<list<string>, int>& lst : buildlists){
            // check if this element already has been converted to units
            if(lst.second <= 0) continue;
            if(lst.second == MAX_TIME) continue;
            // if not convert lst.second to indicate number of targets
            int time_needed = lst.second;
            int no_targets = count_targets(lst.first);
            // the greater the score the better the list
            int score = (rushpush_max_time - time_needed) + (100 * no_targets);
            if(time_needed > rushpush_max_time){
                score = 0;
            }
            score = max(0, score);
            // negative to optimize for best score (sorting problem)
            lst.second = score * -1;
        }
        buildlists.sort(rush_compare);
    }

    int count_targets(list<string> lst){
        int c = 0;
        for(string u: lst){
            if (u == target) ++c;            
        }
        return c;
    }
};