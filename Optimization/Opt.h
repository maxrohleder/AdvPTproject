#pragma once
#include <string>
#include <list>
#include <map>
#include <iostream>
#include "list_builder.h"
#include "../Zerg.h"
#include "../Terran.h"
//#include "../Protoss.h"
#include "../Validator/ValidatorZerg.h"
#include "../Validator/ValidatorTerran.h"

using namespace std;

bool comp(const pair<list<string>, int>& first, const pair<list<string>, int>& second){
    return first.second < second.second;
}

class Opt{
    public:
        Opt(string tech_tree, string target, int amount, bool rush, char race_flag) : path_to_techtree(tech_tree), target(target), amount(amount), rush(rush){
            cout << tech_tree << endl;
            lb = list_builder(path_to_techtree, race_flag);
        }
        Opt(const Opt& o) : path_to_techtree(o.path_to_techtree), target(o.target), amount(o.amount), rush(o.rush){
            lb = list_builder(path_to_techtree, race_flag);
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
            for(int i = 0; i < 100; ++i){
                list<string> new_buildlist = lb.getList(target);
                runBuildList(new_buildlist);
                lb.reset();
            }
            sortBuildLists();
        }


    private:

        void sortBuildLists(){
            buildlists.sort(comp);
        }

        void cutNBest(){
            buildlists.resize(number_best);
        }


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


        list_builder lb;
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

};