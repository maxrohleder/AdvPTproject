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
    return first.second < second.second;
}

class natural_selector{
    private:
    RaceType rt;
    Race evaluator;
    public:
    natural_selector(RaceType r) : rt(r){}
    natural_selector(const &natural_selector n){}
    ~natural_selector(){}

    void sortBuildLists(list<pair<list<string>, int>> *buildlists){
        buildlists->sort(comp);
    }

    void cutNBest(list<pair<list<string>, int>> *buildlists, int n){
        runBuilists(buildlists);
        sortBuildLists(buildlists);
        buildlists->resize(n);
    }

    void runBuilist(list<string> buildlist){
        // evaluate all buildlists and assign a time to them
        // TODO implement properly not python style
        for(int i = 0; i < buildlist.size(); i++)
        {
            list<string> single_list = buildlist[i].first;
            if(r == PROTOSS){
                // TODO create right objects
                evaluator = Protoss(single_list);
            } 
            else if(r == ZERG){
                evaluator = Zerg(single_list);
            }
            else{
                evaluator = Terran(single_list);
            }
            buildlist[i].second = evaluator.getEndTime();
        }
    }

}