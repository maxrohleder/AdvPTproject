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
        int epochs = 1;                     // TODO gescheite werte herausfinden. zu testzwecken jetzt nur ein durchlauf
        int iterations_per_epoch = 1;
        //int number_best = 10;
        int number_to_create_to = 1000;
        //int number_to_mutate_to = 500;
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
        Opt(RaceType race, string tech_tree, string target, int amount, bool rush) : amount(amount), target(target), techtree(tech_tree), rush(rush), r(race){}
        Opt(const Opt& o){}
        ~Opt(){}
        
        void optimize(bool sort = true){
            // init listbuilder
            list_builder lb(target, techtree, amount, r);
            // init natural selector (under construction, dont comment in before it works)
            //natural_selector ns(r);
            // init mating and mutations (under construction)
            //Mutator mu(lb.getMultiple());
            if (analytics){
                // these blocks get compiled away if analytics is false
                analyticsfile.open(analyticsfilepath);
            }
     
            for(size_t i = 0; i < epochs; i++){
                for(int j = 0; j < iterations_per_epoch; j++){
                    int size = buildlists.size();
                    // create 1000 lists
                    lb.appendNLists(buildlists, number_to_create_to-size, sort);
                    //create x buildlists and assign an endtime
                    //lb.appendNLists(buildlists, number_to_create_to-size);
                    //size = buildlists.size();
                    //sort and cut buildlists
                    //ns.cutNBest(buildlists, number_best);
                    //size = buildlists.size();
                    //mutate buildlists
                    //mu.append_n_mutations(buildlists, number_to_mutate_to-size);
                }
                if (analytics){
                    // evaluate effectiveness of algorithm and determine point to stop it
                    pair<list<string>, int> best = buildlists.back();
                    analyticsfile << best.second << endl;
                }
                //repeat
            }
            // close analytics
            if(rush){
                // remove compiler warnings
            }
            if (analytics){
                analyticsfile.close();
            }
        }

        void runPureRandomDebug(bool sort = false){
            list_builder lb(target, techtree, amount, r);
            lb.appendNLists(buildlists, 1000, sort);
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

        void runWinner(){
            pair<list<string>, int> winner = buildlists.front();
            // winner must be valid as no validation is performed
            if(r == ZERG){
                Zerg z(winner.first);
                z.runTest(5000);
            }else if(r == TERRAN){
                Terran t(winner.first);
                t.testRun(5000);
            }else{
                Protoss p(winner.first);
                p.run(5000);
            }
        }

};