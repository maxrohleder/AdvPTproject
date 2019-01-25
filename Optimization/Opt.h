#pragma once
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>

#include "list_builder.h"
//#include "natural_selection.h"
//#include "mutations.h"
#include "global_enums.h"

using namespace std;

class Opt{
private:
        // hyperparameters generic algorithm
        int epochs = 30;
        int iterations_per_epoch = 100;
        int number_best = 10;
        int number_to_create_to = 1000;
        int number_to_mutate_to = 500;
        // target spezific vars
        int amount = 1;
        string target;
        string techtree;
        bool rush;
        RaceType r;
        // datastructures
        list<pair<list<string>, int>> buildlists;
        // analytics vars
        string analyticsfilepath = "ga_analytics.log";
        ofstream analyticsfile;

public:
        Opt(){
            cout << "never used, but have to provide a default constructor" << endl;
            exit(-1);
        }
        Opt(RaceType race, string tech_tree, string target, int amount, bool rush) : r(race), techtree(tech_tree), amount(amount), target(target), rush(rush){}
        Opt(const Opt& o){}
        ~Opt(){}

        void optimize(){
            // init listbuilder
            //list_builder lb(target, techtree, amount, r);
            // init natural selector
            //natural_selector ns(r);
            // init mating and mutations
            //Mutator mu(lb.getMultiple());
            if constexpr(analytics){
                // these blocks get compiled away if analytics is false
                analyticsfile.open(analyticsfilepath);
            }
            
            for(size_t i = 0; i < epochs; i++){
                for(int j = 0; j < iterations_per_epoch; j++){
                    //int size = buildlists.size();
                    list<string> lst = {"probe", "probe", "assimilator", "probe", "pylon", "nexus"};
                    buildlists.push_back(make_pair(lst, i*iterations_per_epoch+j));
                    //create x buildlists and assign an endtime
                    //lb.appendNLists(buildlists, number_to_create_to-size);
                    //size = buildlists.size();
                    //sort and cut buildlists
                    //ns.cutNBest(buildlists, number_best);
                    //size = buildlists.size();
                    //mutate buildlists
                    //mu.append_n_mutations(buildlists, number_to_mutate_to-size);
                }
                if constexpr(analytics){
                    // evaluate effectiveness of algorithm and determine point to stop it
                    pair<list<string>, int> best = buildlists.back();
                    analyticsfile << best.second << endl;
                }
                //repeat
            }
            // assuring the lists are sorted before termination of this method, 
            // so we can either start to optimize again or print the winner, e.g. first entry
            buildlists.sort(comp);
            // close analytics
            if constexpr(analytics){
                analyticsfile.close();
            }
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

        void printWinner(){
            pair<list<string>, int> winner = buildlists.back();
            // TODO print JSON LOG or return list to main and print there idc
            for(string j : winner.first){
                    cout << j << endl;
            }
            cout << "total endtime: " << winner.second << endl;
        }
};