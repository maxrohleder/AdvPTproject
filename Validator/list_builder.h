#include <string>
#include <list>
#include <map>
#include <iostream>
#include "validator_by_file/parser_test.h"
#include "ValidatorZerg.h"
#include "ValidatorTerran.h"


class list_builder{
    public:
        list_builder(const string path_to_techtree, bool debug = false){
            p = parser(path_to_techtree, debug);
        }
        list_builder(const list_builder& lb){
            p = lb.p;
        }
        ~list_builder(){};

    protected:
    void init(){

    }

    void init_Zerg(){
        multiple.push_front("drone");
        multiple.push_front("hatchery");
    }
    parser p;
    list<string> used_only_once_zerg = {"evolution_chamber",
         "spore_crawler", "spawning_pool", "spine_crawler", "roach_warren", 
         "baneling_nest", "hydralisk_den", "infestation_pit", "nydus_network",
         "ultralisk_cavern", "greater_spire", "spire"};
    list<string> once;
    list<string> multiple;


};