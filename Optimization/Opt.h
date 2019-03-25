#pragma once
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>

#include "list_builder.h"
#include "natural_selection.h"
#include "mutations.h"
#include "global_enums.h"

using namespace std;

class Opt{
private:
        // hyperparameters generic algorithm
        int epochs = 200;                        // TODO gescheite werte herausfinden. zu testzwecken jetzt nur ein durchlauf
        int iterations_per_epoch = 20;
        int number_best = 2;                    // should be minimum 2, as mutations use 2 lists
        int number_to_create_to = 20;
        int number_to_mutate_to = 10;
        int stagnation_abbruch = 100;
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
        Opt(RaceType race, string tech_tree, string target, int amount, bool rush) : amount(amount), target(target), techtree(tech_tree), rush(rush), r(race){
            if(rush) rushpush_max_time = amount;
        }
        Opt(const Opt& o){}
        ~Opt(){}

        void setHyper(int ep, int it, int nc, int stag, int nb = 10, int nm = 50, int seed_ = 1){
            epochs = ep;
            iterations_per_epoch = it;
            number_to_create_to = nc;
            stagnation_abbruch = stag;
            number_best = nb;
            number_to_mutate_to = nm;
            seed = seed_;
        }
        
        void optimize(bool sort = true){
            // init listbuilder
            if(rush){
                // in a rush scenario we want to generate buildlists with only 1 target and add more in mutations
                amount = 1;
            }
            srand(seed);
            list_builder lb(target, techtree, amount, r);
            // init natural selector (under construction, dont comment in before it works)
            natural_selector ns(r, rush, target);
            // init mating and mutations (under construction)
            Mutator mu(lb.getMultiple(), lb.getParser(), r, amount, target);
            if (analytics){
                // these blocks get compiled away if analytics is false
                analyticsfile.open(analyticsfilepath);
            }
            int stagnation = 0;
            int best_runtime = -1;
     
            for(size_t i = 0; i < epochs; i++){
                //cout << "epoch: " << i << endl; // test
                for(int j = 0; j < iterations_per_epoch; j++){
                    //seed += 69;
                    //srand(seed);
                    int size = buildlists.size();
                    // create 1000 lists
                    lb.appendNLists(buildlists, number_to_create_to-size, sort);
                    //create x buildlists and assign an endtime
                    size = buildlists.size();
                    //sort and cut buildlists
                    ns.cutNBest(buildlists, number_best, amount);
                    size = buildlists.size();
                    //mutate buildlists
                    mu.append_n_mutations(buildlists, number_to_mutate_to-size, rush, target);
                }
                // alternative abbruch kriterion
                pair<list<string>, int> best = buildlists.front();
                if(best_runtime == best.second){
                    ++stagnation;
                } else if (best_runtime == -1){
                    best_runtime = best.second;                    
                } else{
                    stagnation = 0;
                    best_runtime = best.second;
                }
                // evaluate effectiveness of algorithm and determine point to stop it
                if (analytics){
                    analyticsfile << "iteration: " << (i+1)*iterations_per_epoch << "\ttime: " << best.second << " (" << stagnation << "/" << stagnation_abbruch << ")" << endl;
                }
                // break if best time doesnt change for 20 iterations
                if(stagnation >= stagnation_abbruch){
                    break;  
                }
            }
            // close analytics
            if(rush){
                // remove compiler warnings
            }
            if (analytics){
                analyticsfile << "stagnation count: " << stagnation << endl;
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

        void printWinner(){
            cout << buildlists.begin()->second << ": ";
            int target_count = 0;
            for(auto& i : buildlists.begin()->first){
                cout << i << " ";
                if (i == target) ++target_count;
            }
            cout << endl;
            cout << target_count << endl;
            cout << "seed: " << seed << endl;
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

        void find_best_hyper(int num_seeds){
            analytics = false;
            ofstream hyperFile;
            hyperFile.open("ga_analytics.log");
            int nc=100; int nb=25; int nm=50; int ep=50; int stag=30; int it=20; int seed_=1;
            for(int i = 1; i < num_seeds; i++)
            {
                setHyper(ep, it, nc, stag, nb, nm, seed_+5*i);
                hyperFile << "seed: " << seed_+5*i << endl;
                optimize();

                hyperFile <<"achieved time: " << buildlists.begin()->second;
                int target_count = 0;
                for(auto& i : buildlists.begin()->first){
                    hyperFile << i << " ";
                    if (i == target) ++target_count;
                }
            }
        }

        int countIllegalLists(){
            int count = 0;
            for(auto & i : buildlists){
                if(i.second == MAX_TIME){
                    ++count;
                }
            }
            return count;
        }
};