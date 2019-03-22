#include <string>
#include <list>
#include <map>
#include <iostream>
#include <vector>
#include <iterator>

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
// https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_mutation.htm
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
    int maxTargetInsert = 5;
    
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
        // common ground cross breeding (length reduction)
        return list1;
    }

    list<string> crossBreedSimple(list<string> list1, list<string> list2){
        size_t length_min = min(list1.size(), list2.size());
        list<string> l = {};
        int pos = rand() % length_min;
        auto it1 = list1.begin();
        auto it2 = list2.begin();
        for(int i = 0; i < length_min; ++i, ++it1, ++it2){
            if(i < pos){
                l.push_back(*it1);
            }else{
                l.push_back(*it2);
            }
        }
        return l;
    }

    list<string> singleSwap(list<string> list1, list<string> list2, string target){
        int pos1 = rand() % (list1.size() - 1);
        int pos2 = rand() % list2.size();
        auto it1 = list1.begin();
        auto it2 = list2.begin();
        for(size_t i = 0; i < pos1; ++i){
            ++it1; 
        }
        for(size_t i = 0; i < pos2; ++i){
            ++it2; 
        }
        if(*it1 == target || *it2 == target){
            return list1;
        }
        *it1 = *it2;
        return list1;        
    }

    list<string> cutOneOut(list<string> l){
        int pos = rand() % (l.size() - 1);
        auto it = l.begin();
        for(size_t i = 0; i < pos; ++i){
            ++it; 
        }
        l.erase(it);
        return l;
    }

    list<string> mutate(list<string> list1){
        // TODO insert an item from multiple at a random position TOIMPLEMENT
        int pos = rand() % multiple->size();
        string insert_unit = multiple->at(pos);
        int pos2 = rand()%list1.size();
        auto init_pos = list1.begin();
        advance(init_pos, pos2);
        list1.insert(init_pos, worker);
        return list1;
    }

    // dedicated to maximize rush scenario
    list<string> moreWorkersAtRandomPositions(list<string> list1){
        int workers_to_insert = rand()%worker_insertion;
        for(int i = 0; i < workers_to_insert; i++){
            int pos2 = rand()%list1.size();
            auto init_pos = list1.begin();
            advance(init_pos, pos2);
            list1.insert(init_pos, worker);
        }
        return list1;
    }

    list<string> moreTargetUnitsAfterDependency(list<string> list1, string target){
        // essential to a rush scenario
        int numberOfTargetsToInsert = rand()%maxTargetInsert;
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
    list<string>::iterator getMinInsertPosition(list<string> &l, string d, string p){
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
                list<pair<list<string>, int>>::iterator list1 = buildlists.begin();
                advance(list1, l1);
                //list<string> res = moreTargetUnitsAfterDependency(list1->first, target);
                int choose = rand() % 3;
                list<string> res = {};
                if(choose == 0){
                    res = mutateCopyToRandom(list1->first);
                }else if(choose == 1){
                    res = cutOneOut(list1->first);
                }else{
                    res = mutateOverwriteAtRandom(list1->first, target);
                }
                runAndInsertList(buildlists, res);
            }
        }

        for(int i = 0; i < n; i++)
        {
            int chance = rand() % 4;
            int l1 = rand() % buildlists.size();
            auto list1 = buildlists.begin();
            advance(list1, l1);
            // new buildlist created by one of the following methods
            list<string> res;
            int l2 = rand() % buildlists.size();
            auto list2 = buildlists.begin();
            advance(list2, l2);
            if(chance == 0){
                //res = cross_breed(list1->first, list2->first);
                res = singleSwap(list1->first, list2->first, target);
                runAndInsertList(buildlists, res);
            }
            else if(chance == 1){
                res = mutate(list1->first);
            }
            else if(chance == 2){
                res = moreWorkersAtRandomPositions(list1->first);
            }
            else if(chance == 3){
                res = crossBreedSimple(list1->first, list2->first);
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
            if(!validate(p, false)){
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

    //copy one element to random position
    list<string> mutateCopyToRandom(list<string> l){
        size_t length = l.size();
        size_t pos_from = rand() % length;
        size_t pos_to = rand() % length;
        auto it_to = l.begin();
        for(size_t i = 0; i < pos_to; ++i){
            ++it_to;
        }
        auto it_from = l.begin();
        for(size_t i = 0; i < pos_from; ++i){
            ++it_from; 
        }
        string to_insert = *it_from;
        l.insert(it_to, to_insert);
        return l;
    }

    //overwrite element at random position
    list<string> mutateOverwriteAtRandom(list<string> l, string target){
        size_t length = l.size();
        size_t pos_from = rand() % length;
        size_t pos_to = rand() % (length - 1);
        auto it_to = l.begin();
        for(size_t i = 0; i < pos_to; ++i){
            ++it_to;
        }
        auto it_from = l.begin();
        for(size_t i = 0; i < pos_from; ++i){
            ++it_from; 
        }
        string to_insert = *it_from;
        if(to_insert == target){
            return l;
        }
        *it_to = to_insert;
        return l;
    }
};