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

bool single(pair<list<string>, int> first, pair<list<string>, int> second)
{
    if (first.first.size() != second.first.size())
        return false;
    auto it1 = first.first.begin();
    auto it2 = second.first.begin();
    for (int i = 0; i < first.first.size(); ++i, ++it1, ++it2)
    {
        if (*it1 != *it2)
            return false;
    }
    return true;
}

bool cutByTime(pair<list<string>, int> first, pair<list<string>, int> second){
    return first.second == second.second;
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
class natural_selector
{
  private:
    RaceType rt;
    bool rush;
    string target;
    Race evaluator;

  public:
    natural_selector(RaceType r, bool rush, string target) : rt(r), rush(rush), target(target) {}
    //natural_selector(const &natural_selector n){}
    ~natural_selector() {}

    void sortBuildLists(list<pair<list<string>, int>> &buildlists)
    {
        for (auto const &i : buildlists)
        {
            buildlists.sort(comp);
        }
    }

    void medianCut(list<pair<list<string>, int>> &buildlists)
    {
        // TODO calculate median and cut all lists above. (ignore all INT_MAX times) (performes well on outliers)
    }

    void cutNBest(list<pair<list<string>, int>> &buildlists, int n, int amount, bool median_cut = false)
    {
        if (rush)
        {
            // overwrites second integer with number of targets * -1 (to be able to differenciate between time and amount)
            sort_wrt_amout(buildlists);
        }else{
            buildlists.sort(compWithSize);
        }
        if (median_cut)
        {
            medianCut(buildlists);
        }
        else
        {
            buildlists.unique(single);
            //buildlists.unique(cutByTime);
            buildlists.resize(n);
        }
    }

    void sort_wrt_amout(list<pair<list<string>, int>> &buildlists)
    {
        for (pair<list<string>, int> &lst : buildlists)
        {
            // check if this element already has been converted to units
            if (lst.second <= 0)
                continue;
            if (lst.second == MAX_TIME)
                continue;
            // if not convert lst.second to indicate number of targets
            int time_needed = lst.second;
            int no_targets = count_targets(lst.first);
            // the greater the score the better the list
            int score = (rushpush_max_time - time_needed) + (100 * no_targets);
            if (time_needed > rushpush_max_time)
            {
                score = 0;
            }
            score = max(0, score);
            // negative to optimize for best score (sorting problem)
            lst.second = score * -1;
        }
        buildlists.sort(rush_compare);
    }

    int count_targets(list<string> lst)
    {
        int c = 0;
        for (string u : lst)
        {
            if (u == target)
                ++c;
        }
        return c;
    }
};