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
// neccesary for push scenario dependency info
#include "parser.h"

using namespace std;

// create n mutations
// cross breeding
// wenn zeit knapp drüber ist eine einheit entfernen
// mutation (zufällige mod an einer liste)
class Mutator{
    private:
    vector<string> *multiple = NULL;
    par *p = NULL;
    RaceType race_flag;
    string worker;
    int worker_insertion = 4;
    
    public: 
    Mutator(){}
    Mutator(vector<string> &multi, par &dep_info, RaceType r): multiple(&multi), p(&dep_info), race_flag(r) {
        if(r == PROTOSS){
            worker = "probe";
        } else if(r == TERRAN){
            worker = "scv";
        }else{
            worker = "drone";
        }
    }
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
        int pos = rand() % multiple->size();
        string insert_unit = multiple->at(pos); // TODO multiple[pos]
        int pos2 = rand()%list1.size();
        auto init_pos = list1.begin();
        advance(init_pos, pos2);
        list1.insert(init_pos, worker);
        return list1;
    }

    // dedicated to maximize rush scenario
    list<string> moreWorkersAtRandomPositions(list<string> list1){
        for(int i = 0; i < worker_insertion; i++){
            int pos2 = rand()%list1.size();
            auto init_pos = list1.begin();
            advance(init_pos, pos2);
            list1.insert(init_pos, worker);
        }
        return list1;
    }

    list<string> moreTargetUnitsAfterDependency(list<string> list1, string target){
        // essential to a rush scenario
        int numberOfTargetsToInsert = rand()%5;
        lineObj* lo = p->get_obj(target);
        // determine min position where its okay to insert (dependency etc)
        list<string>::iterator start = getMinInsertPosition(list1, lo->dependency, lo->produced_by);
        int maxOffset = distance(start, list1.end());
        // insert elements at random locations in range [start, list.end()]
        for(int i = 0; i < numberOfTargetsToInsert; i++){
            auto insertion_point = start;
            // range from [0, maxOffset-1] --> [1, maxOffset]
            int rand_advance = rand()%maxOffset;
            advance(insertion_point, rand_advance + 1);
            list1.insert(insertion_point, target);
        }
        return list1;
    }

    /// if this returns a .end() then some dependency or producer is not found!
    list<string>::iterator getMinInsertPosition(list<string> l, string d, string p){
        // if no dependency or producer is set. we can insert from beginning on
        if(d == "NONE" && p == "NONE") return l.begin();

        // return the most advanced, yet not .end() iterator
        list<string>::iterator d_location = find(l.begin(), l.end(), d);
        list<string>::iterator p_location = find(l.begin(), l.end(), p);

        // if one is NONE, then were looking for the other
        if(d == "NONE") return p_location;
        if(p == "NONE") return d_location;

        // if d is further advanced than p, yet not at .end() then return d
        if(distance(p_location, d_location) >= 0){
            return d_location;
        }
        return p_location;        
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
                list<string> res = moreTargetUnitsAfterDependency(list1->first, target);
                runAndInsertList(buildlists, res);
            }
        }

        for(int i = 0; i < n; i++)
        {
            int chance = rand() % 3;
            int l1 = rand() % buildlists.size();
            auto list1 = buildlists.begin();
            advance(list1, l1);
            // new buildlist created by one of the following methods
            list<string> res;
            if(chance == 0){
                int l2 = rand() % buildlists.size();
                auto list2 = buildlists.begin();
                advance(list2, l2);
                res = cross_breed(list1->first, list2->first);
                runAndInsertList(buildlists, res);
            }
            else if(chance == 1){
                res = mutate(list1->first);
            }
            else if(chance == 2){
                res = moreWorkersAtRandomPositions(list1->first);
            }
            runAndInsertList(buildlists, res);
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