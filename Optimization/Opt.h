#pragma once
#include <string>
#include <list>
#include <map>
#include <iostream>

#include "list_builder.h"
<<<<<<< HEAD
#include "natural_selection.h"
#include "mutations.h"
#include "global_enums.h"
=======
#include "../Zerg.h"
#include "../Terran.h"
//#include "../Protoss.h"
#include "../Validator/ValidatorZerg.h"
#include "../Validator/ValidatorTerran.h"
>>>>>>> f4120ce07d1e03c54b85120a6cc5ed02ae05f53a

using namespace std;

class Opt{
<<<<<<< HEAD
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
=======
    public:
        Opt(string tech_tree, string target, int amount, bool rush, char race_flag) : path_to_techtree(tech_tree), target(target), amount(amount), rush(rush), race_flag(race_flag){
            //cout << tech_tree << endl;
            //lb = list_builder(path_to_techtree, race_flag);
        }
        Opt(const Opt& o) : path_to_techtree(o.path_to_techtree), target(o.target), amount(o.amount), rush(o.rush){
            //lb = list_builder(path_to_techtree, race_flag);
        }
        ~Opt(){}

        list<string> optimize(){
            //create x buildlists
            //sort buildlists
            //cut buildlists
            //mutate buildlists
            //repeat
            list<string> r = {"abc", "abc"};
            return r;
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

        void runPureRandomDebug(){
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


    private:
>>>>>>> f4120ce07d1e03c54b85120a6cc5ed02ae05f53a

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

<<<<<<< HEAD
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
=======

        //maybe without race as type but with flag or something
        /*
        void runBuilist(list<string> buildlist){
            r.getEndTime(1000);
        }
        */
       
        void runBuildList(list<string> buildlist){
            //run Validator on List
            //if not valid time -> max_time
            //if valid run forward 
            //time -> runtime
            //push into buildLists
            if(race_flag == 'z'){
                ZergChecker zc = ZergChecker();
                bool valid = zc.run(buildlist);
                int end_time = max_time;
                if(valid){
                    Zerg z(buildlist);
                    end_time = z.getEndTime(50000);
                }
                buildlists.push_back(make_pair(buildlist, end_time));
            }
           
          
        }


        //list_builder lb;
        int epochs = 30;
        int steps = 1000;
        list<pair<list<string>, int>> buildlists;
        int number_best = 10;
        //Race& r;
        int max_time = INT32_MAX;
        string path_to_techtree = "";
        string target = "";
        int amount = 1;
        bool rush = false;
        char race_flag = 'd';

>>>>>>> f4120ce07d1e03c54b85120a6cc5ed02ae05f53a
};