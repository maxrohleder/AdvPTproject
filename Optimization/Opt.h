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
        list_builder lb;
        Mutator mu;
        natural_selector ns;

        int epochs = 30;
        int number_best = 10;
        int number_to_create_to = 1000;
        int number_to_mutate_to = 500;
        RaceType r;

        list<pair<list<string>, int>> buildlists;

public:
        Opt(){
            cout << "never used, but have to provide a default constructor" << endl;
            exit(-1);
        }
        Opt(RaceType race, string tech_tree, string target, int amount, bool rush) : r(race){
            // init listbuilder
            lb = list_builder(target, tech_tree);
            // init natural selector
            ns = natural_selector(race);
            // init mating and mutations
            mu = Mutator(lb.get_multiple());
        }
        Opt(const Opt& o){}
        ~Opt(){}

        list<string> optimize(){
            
            for(size_t i = 0; i < epochs; i++){
                int size = buildlists.size();
                //create x buildlists
                lb.append_n_lists(&buildlists, number_to_create_to-size);
                size = buildlists.size();
                //sort and cut buildlists
                ns.cutNBest(&buildlists, number_best);
                size = buildlists.size();
                //mutate buildlists
                mu.append_n_mutations(&buildlists, number_to_mutate_to-size)
                //repeat
            }
        }
};