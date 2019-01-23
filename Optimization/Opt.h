#pragma once
#include <string>
#include <list>
#include <map>
#include <iostream>

#include "list_builder.h"
#include "natural_selection.h"
#include "mutations.h"
#include "global_enums.h"

using namespace std;

class Opt{
private:
        int epochs = 30;
        int number_best = 10;
        int number_to_create_to = 1000;
        int number_to_mutate_to = 500;
        int amount = 1;
        string target;
        string techtree;
        bool rush;
        RaceType r;
        list<pair<list<string>, int>> buildlists;

public:
        Opt(){
            cout << "never used, but have to provide a default constructor" << endl;
            exit(-1);
        }
        Opt(RaceType race, string tech_tree, string target, int amount, bool rush) : r(race), techtree(tech_tree), amount(amount), target(target), rush(rush){}
        Opt(const Opt& o){}
        ~Opt(){}

        list<string> optimize(){
            // init listbuilder
            list_builder lb(target, techtree);
            // init natural selector
            natural_selector ns(r);
            // init mating and mutations
            Mutator mu(lb.getMultiple());
            
            for(size_t i = 0; i < epochs; i++){
                int size = buildlists.size();
                //create x buildlists and assign an endtime
                lb.appendNLists(buildlists, number_to_create_to-size);
                size = buildlists.size();
                //sort and cut buildlists
                ns.cutNBest(buildlists, number_best);
                size = buildlists.size();
                //mutate buildlists
                mu.append_n_mutations(buildlists, number_to_mutate_to-size);
                //repeat
            }
        }

        void runBuildList(list<string> buildlist){
            //run Validator on List
            //if not valid time -> max_time
            //if valid run forward 
            //time -> runtime
            //push into buildLists
            if(r == ZERG){
                ZergChecker zc = ZergChecker();
                bool valid = zc.run(buildlist);
                int end_time = lstat::MAX_TIME;
                if(valid){
                    Zerg z(buildlist);
                    end_time = z.getEndTime(50000);
                }
                buildlists.push_back(make_pair(buildlist, end_time));
            }
           
          
        }

        void runPureRandomDebug(){}
        /*
            list_builder lb = list_builder(path_to_techtree, race_flag, amount);
            for(int i = 0; i < 100; ++i){
                list<string> new_buildlist = lb.getList(target);
                runBuildList(new_buildlist);
                lb.reset();
            }
            sortBuildLists();
        }

        list<string> getBestList(){
            return buildlists.front().first;
        }
        */

        void printBuildlists(){
            for(auto& i : buildlists){
                cout << i.second << ": ";
                for(auto& j : i.first){
                    cout << j << " ";
                }
                cout << endl;
            }
        }
};