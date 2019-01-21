#include <string>
#include <list>
#include <map>
#include <iostream>
#include "validator_by_file/parser_test.h"
#include "ValidatorZerg.h"
#include <algorithm>
#include <stdlib.h>
#include <vector>

//this will generate a buildlist out of digList for dependencies, once for units only needed once and multiple for units needed multiple times
class list_builder{
    public:
        list_builder(const string path_to_techtree, char rf, bool debug = false) : race_flag(rf){
            init(rf);
            p = parser(path_to_techtree, debug);
        }
        list_builder(const list_builder& lb) : p(lb.p), race_flag(lb.race_flag){
            init(lb.race_flag);
            p = lb.p;
        }
        ~list_builder(){};

        list_builder operator=(const list_builder& lb){
            p = lb.p;
            used_only_once = lb.used_only_once;
            once = lb.once;
        }

        void runDebug(string start){
            buildDigList(start);
            for(int i = 0; i < 10; ++i){
                string r = getRandomUnit();
                if(r != ""){
                    addToBuildlist(r);
                    printAllLists();
                }else{
                    --i;
                }
            }
        }

    protected:
    void init(char rf){
        srand(0);
        if(rf == 'z'){
            used_only_once = used_only_once_zerg;
            initZerg();
        }
    }

    void initZerg(){
        multiple.push_back("drone");
        multiple.push_back("hatchery");
        multiple.push_back("overlord");
        once.push_back("extractor");
        once.push_back("extractor");
    }

    //add 2 vespene producers
    void push_vespene(){
        if(race_flag == 'z'){
            once.push_back("extractor");
            once.push_back("extractor");
        }
    }

    //build digList to get dependencies etc right
    void buildDigList(string name){
        lineObj* lo = p.get_obj(name);
        if(lo == NULL){
            cout << "invalid name: " << name << endl;
            exit(1);
        }
        if(lo->dependency != "NONE"){
            buildDigList(lo->dependency);
        }
        if(lo->produced_by != "NONE"){
            buildDigList(lo->produced_by);
        }
        if(lo->vespene) vespene = true;
        digList.push_back(name);
    }

    //add producable to either once or multiple
    void addToProducable(string name){
        if(find(used_only_once.begin(), used_only_once.end(), name) == used_only_once.end()){
            multiple.push_back(name);
        }
    }

    //helper to decide if already in once
    void addToOnce(string name){
        if(find(once.begin(), once.end(), name) == once.end()){
            once.push_back(name);
        }
    }

    void printDigList(){
        cout << "digList: ";
        for(auto i : digList){
            cout << i << " ";
        }
        cout << endl;
    }

    void printAllLists(){
        printDigList();
        cout << "once: ";
        for(auto i : once){
            cout << i << " ";
        }
        cout << endl;
        cout << "multiple: ";
        for(auto i : multiple){
            cout << i << " ";
        }
        cout << endl;
        cout << "buildlist: ";
        for(auto i : buildlist){
            cout << i << " ";
        }
        cout << endl << endl;
    }

    string getFromMultiple(){
        int pos = rand() % multiple.size();
        return multiple[pos];
    }

    string getFromOnce(){
        if(once.empty()) return ""; 
        int pos = rand() % once.size();
        string r = once[pos];
        once.erase(once.begin() + pos);
        return r;
    }

    string getFromDigList(){
        if(digList.empty()) return "";
        string r = digList[0];
        digList.erase(digList.begin());
        return r;
    }

    string getRandomUnit(){
        int list_to_use = rand() % 3;
        if(list_to_use == 0){
            return getFromMultiple();
        }else if(list_to_use == 1){
            return getFromOnce();
        }else{
            string new_unit = getFromDigList();
            addToProducable(new_unit);
            return new_unit;
        }
    }

    void addToBuildlist(string name){
        buildlist.push_back(name);
    }

    parser p;
    list<string> used_only_once_zerg = {"evolution_chamber",
         "spore_crawler", "spawning_pool", "spine_crawler", "roach_warren", 
         "baneling_nest", "hydralisk_den", "infestation_pit", "nydus_network",
         "ultralisk_cavern", "greater_spire", "spire"};
    list<string>& used_only_once = used_only_once_zerg;
    vector<string> once;
    vector<string> multiple;
    vector<string> digList;
    bool vespene = false;
    char race_flag = 'd';
    list<string> buildlist;

};