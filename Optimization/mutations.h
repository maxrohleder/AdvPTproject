#include <string>
#include <list>
#include <map>
#include <iostream>
#include <vector>

#include "global_enums.h"
// only neccessary to get an endtime
#include "../Zerg.h"
#include "../Protoss.h"
#include "../Terran.h"
#include "../Terran/parser_terran.h"
#include "../Validator/ValidatorZerg.h"

using namespace std;

// create n mutations
// cross breeding
// wenn zeit knapp drüber ist eine einheit entfernen
// mutation (zufällige mod an einer liste)
class Mutator{
    private:
    vector<string> *multiple = NULL;
    RaceType race_flag;
    
    public:
    Mutator(){}
    Mutator(vector<string> *multi, RaceType r): multiple(multi), race_flag(r){}
    Mutator(const Mutator &m){}
    ~Mutator(){}

    list<string> cross_breed(list<string> list1, list<string> list2){
        // TODO mix through two lists creating a new one
        // could be either equally long or shorter/longer, but should only be able 
        // to remove elements not beeing in multiple
        return list1;
    }

    list<string> mutate(list<string> list1){
        // TODO insert an item from multiple at a random position TOIMPLEMENT
        return list1;
    }

    // dedicated to maximize rush scenario
    list<string> moreWorkersAtRandomPositions(list<string> list1){
        // TODO insert workers at random positions
        return list1;
    }

    void append_n_mutations(list<pair<list<string>, int>> &buildlists, int n, bool rush, string target){

        // always needed in a rush scenario, as "as many as possible" is not covered by list_builder
        if(rush){
            int ndrittel = (int)((double) n / 3);
            n = n-ndrittel;
            for(int i = 0; i < ndrittel; i++){
                int l1 = rand() % buildlists.size();
                auto list1 = buildlists.begin();
                advance(list1, l1);
                list<string> res = moreWorkersAtRandomPositions(list1->first);
                runAndInsertList(buildlists, res);
            }
        }

        for(int i = 0; i < n; i++)
        {
            int chance = rand() % 2;
            if(chance == 0){
                // TODO randomly cross breed (and make this nicer :'D)
                int l1 = rand() % buildlists.size();
                int l2 = rand() % buildlists.size();
                auto list1 = buildlists.begin();
                auto list2 = buildlists.begin();
                advance(list1, l1);
                advance(list2, l2);
                list<string> res = cross_breed(list1->first, list2->first);
                runAndInsertList(buildlists, res);
            }
            else if(chance == 1){
                // TODO randomly add mutations
                int l1 = rand() % buildlists.size();
                auto list1 = buildlists.begin();
                advance(list1, l1);
                list<string> res = mutate(list1->first);
                runAndInsertList(buildlists, res);
            }
        }
    }

    void runAndInsertList(list<pair<list<string>, int>>& buildlists, list<string> &bl){
        int time = MAX_TIME;
        bool valid = true;
        // TODO generalize to all races
        if(race_flag == ZERG){
            ZergChecker zc = ZergChecker();
            valid = zc.run(bl);
            if(valid){
                Zerg z(bl);
                time = z.getEndTime(5000);
            }
        }else if(race_flag == TERRAN){
            parser_terran p (path_techtree_terran, bl, false);
            if(validate(p, false)){
                Terran t(bl);
                time = t.getEndTime(5000);
            }
        }else {
            parser p (path_techtree_protoss, bl, false);
            if(!validate(p, false)){
                Protoss P(bl);
                time = P.getEndTime(5000);
            }
        }
        buildlists.push_back(make_pair(bl, time));        
    }


};