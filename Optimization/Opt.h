#pragma once
#include <string>
#include <list>
#include <map>
#include <iostream>

#include "list_builder.h"
//#include "natural_selection.h"
//#include "mutations.h"
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
            //natural_selector ns(r);
            // init mating and mutations
            //Mutator mu(lb.getMultiple());
            
            for(size_t i = 0; i < epochs; i++){
                int size = buildlists.size();
                //create x buildlists and assign an endtime
                lb.appendNLists(buildlists, number_to_create_to-size);
                size = buildlists.size();
                //sort and cut buildlists
                //ns.cutNBest(buildlists, number_best);
                size = buildlists.size();
                //mutate buildlists
                //mu.append_n_mutations(buildlists, number_to_mutate_to-size);
                //repeat
            }
            list<string> useless_warning_workaround = {"fuckthisshit", "whothefuckneedsreturnstatements"};
            return useless_warning_workaround;
        }
        

        void runPureRandomDebug(bool sort = false){
            list_builder lb(target, techtree, amount, r);
            lb.appendNLists(buildlists, 1000, sort);
            printBuildlists();
        }
        

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