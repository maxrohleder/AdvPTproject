#include <string>
#include <list>
#include <map>
#include <iostream>
#include "list_builder.h"
#include "../Zerg.h"
#include "../Terran.h"
#include "../Protoss.h"
#include "../Validator/ValidatorZerg.h"
#include "../Validator/ValidatorTerran.h"

using namespace std;

bool comp(const pair<list<string>, int>& first, const pair<list<string>, int>& second){
    return first.second < second.second;
}

class Opt{
    public:
        Opt(string tech_tree = "", string target = "", int amount = 1, bool rush = true){}
        Opt(const Opt& o){}
        ~Opt(){}

        list<string> optimize(){
            //create x buildlists
            //sort buildlists
            //cut buildlists
            //mutate buildlists
            //repeat

        }

    private:

        void sortBuildLists(){
            buildlists.sort(comp);
        }

        void cutNBest(){
            buildlists.resize(number_best);
        }

        void runBuilist(list<string> buildlist){
            r.getEndTime(1000);
        }



        list_builder lb;
        int epochs = 30;
        int steps = 1000;
        list<pair<list<string>, int>> buildlists;
        int number_best = 10;
        Race& r;


};