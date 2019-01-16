#include <string>
#include <list>
#include <map>
#include <iostream>
#include "validator_by_file/parser_test.h"
#include "ValidatorZerg.h"
#include "ValidatorTerran.h"
#include <algorithm>

//this will generate a buildlist out of digList for dependencies, once for units only needed once and multiple for units needed multiple times
class list_builder{
    public:
        list_builder(const string path_to_techtree, char rf = 'd', bool debug = false) : race_flag(rf){
            p = parser(path_to_techtree, debug);
        }
        list_builder(const list_builder& lb) : p(lb.p), race_flag(lb.race_flag){}
        ~list_builder(){};

    protected:
    void init(){

    }

    void init_Zerg(){
        multiple.push_front("drone");
        multiple.push_front("hatchery");
        multiple.push_front("overlord");
        once.push_front("extractor");
        once.push_front("extractor");
    }

    //add 2 vespene producers
    void push_vespene(){
        if(race_flag == 'z'){
            once.push_front("extractor");
            once.push_front("extractor");
        }
    }

    //build diglist to get dependencies etc right
    void buildDigList(string name, const lineObj& lo){
        if(lo.dependency != "NONE"){
            buildDigList(lo.dependency, p.get_obj(lo.dependency));
        }
        if(lo.produced_by != "NONE"){
            buildDigList(lo.produced_by, p.get_obj(lo.produced_by));
        }
        if(lo.vespene) vespene = true;
        diglist.push_back(name);
    }

    //add producable to either once or multiple
    void addToProducable(string name){
        if(race_flag == 'z'){
            if(find(used_only_once_zerg.begin(), used_only_once_zerg.end(), name) == used_only_once_zerg.end()){
                multiple.push_front(name);
            }else{
                addToOnce(name);
            }
        }
    }

    //helper to decide if already in once
    void addToOnce(string name){
        if(find(once.begin(), once.end(), name) == once.end()){
            once.push_front(name);
        }
    }


    parser p;
    list<string> used_only_once_zerg = {"evolution_chamber",
         "spore_crawler", "spawning_pool", "spine_crawler", "roach_warren", 
         "baneling_nest", "hydralisk_den", "infestation_pit", "nydus_network",
         "ultralisk_cavern", "greater_spire", "spire"};
    list<string> once;
    list<string> multiple;
    list<string> diglist;
    bool vespene = false;
    char race_flag = 'd';

};