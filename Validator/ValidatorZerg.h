#include <iostream>
#include <list>
#include <map>
#include <string>
#include "Validator.h"

using namespace std;

//struct for dependency vars 
struct depZerg{
    int lair = 0;
    int hive = 0;
    int hydralisk_den = 0;
    int spawning_pool = 0;
    int baneling_nest = 0;
    int roach_warren = 0;
    int infestation_pit = 0;
    int ultralisk_cavern = 0;
    int greater_spine = 0;
    int spire = 0;
    int evolution_chamber = 0;
    int nydus_network = 0;
    int extractor = 0;
};

//struct for resource vars
struct resZerg{
    int drones = 6;
    int overlord = 1;
    int zergling = 0;
    int corruptor = 0;
    int bases = 1;
    int hatchery = 1;
    int lair = 0;
    int supply = 10;
    int extractor_slots = 0; //free extractor slots

};


class ZergChecker{
    public:
        ZergChecker(){
            cout << "init start" << endl;
            initDependency();
            initResources();
            Validator val(depMap, resMap);
        }
        ZergChecker(const ZergChecker* zc){}
        ~ZergChecker(){}

        //run with textfile
        bool run(string file_name){
            //TODO
            return false;
        }
        //run with parsed buildlist
        bool run(const list<string> buildlist){
            return val.run(&buildlist);
        }
        //debug run
        bool runDebug(const list<string> buildlist){
            cout << "DEBUG TEST: " << endl;
            cout << "input list: " << endl;
            for(auto i : buildlist){
                cout << i << endl;
            }
            return val.runDebug(&buildlist);
        }



    private:
        depZerg dependencys;
        resZerg resources;
        map<string, depObj> depMap;
        map<string, resObj> resMap;
        Validator val = NULL;
        void initDependency(){
            depMap["drone"] = depObj();
            depMap["hatchery"] = depObj();
            depMap["zergling"] = depObj({&dependencys.spawning_pool});
            depMap["spawning_pool"] = depObj({}, &dependencys.spawning_pool);
        }
        void initResources(){
            resMap["drone"] = resObj({resourceToUpdate(&resources.drones, 1), resourceToUpdate(&resources.supply, -1)});
            resMap["hatchery"] = resObj({resourceToUpdate(&resources.hatchery, 1), resourceToUpdate(&resources.bases, 1),
                 resourceToUpdate(&resources.extractor_slots, 2), resourceToUpdate(&resources.supply, 2)});
            resMap["spawning_pool"] = resObj();
        }
};