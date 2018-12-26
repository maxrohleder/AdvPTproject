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
    int supply = 4;
    int extractor_slots = 2; //free extractor slots
    int spire = 0;

};


class ZergChecker{
    public:
        ZergChecker(){
            initDependency();
            initResources();
            val = Validator(depMap, resMap);
        }
        ZergChecker(const ZergChecker* zc){}
        ~ZergChecker(){}

        //run with textfile
        bool run(string file_name){
            list<string> buildlist;
            ifstream file;
            string unit_name;
            file.open(file_name);
            if(file.is_open()){
               while(getline(file, unit_name)){
                    if(depMap.find(unit_name) == depMap.end()){
                        cerr << "bad unitname: " << unit_name << endl;
                        exit(1);
                    }
                    buildlist.push_back(unit_name);
                }
                file.close();
            }else{
                cerr << "could not read file" << endl;
                exit(-1);
            }
            return val.run(&buildlist);
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

        bool runDebug(string file_name){
            list<string> buildlist;
            ifstream file;
            string unit_name;
            file.open(file_name);
            if(file.is_open()){
               while(getline(file, unit_name)){
                    if(depMap.find(unit_name) == depMap.end()){
                        cerr << "bad unitname: " << unit_name << endl;
                        exit(1);
                    }
                    buildlist.push_back(unit_name);
                }
                file.close();
            }else{
                cerr << "could not read file" << endl;
                exit(-1);
            }
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
        Validator val;
        void initDependency(){
            depMap["drone"] = depObj();
            depMap["hatchery"] = depObj();
            depMap["zergling"] = depObj({&dependencys.spawning_pool});
            depMap["spawning_pool"] = depObj({}, &dependencys.spawning_pool);
            depMap["overlord"] = depObj();
            depMap["overseer"] = depObj({&dependencys.lair});
            depMap["hydralisk"] = depObj({&dependencys.hydralisk_den});
            depMap["queen"] = depObj({&dependencys.spawning_pool});
            depMap["baneling"] = depObj({&dependencys.baneling_nest});
            depMap["roach"] = depObj({&dependencys.roach_warren});
            depMap["infestor"] = depObj({&dependencys.infestation_pit});
            depMap["nydus_worm"] = depObj({&dependencys.nydus_network});
            depMap["ultralisk"] = depObj({&dependencys.ultralisk_cavern});
            depMap["brood_lord"] = depObj({&dependencys.greater_spine});
            depMap["corruptor"] = depObj({&dependencys.spire});
            depMap["mutalisk"] = depObj({&dependencys.spire});
            depMap["evolution_chamber"] = depObj();
            depMap["spore_crawler"] = depObj({&dependencys.evolution_chamber});
            depMap["lair"] = depObj({&dependencys.spawning_pool}, &dependencys.lair);
            depMap["extractor"] = depObj();
            depMap["spine_crawler"] = depObj({&dependencys.spawning_pool});
            depMap["roach_warren"] = depObj({&dependencys.spawning_pool}, &dependencys.roach_warren);
            depMap["baneling_nest"] = depObj({&dependencys.spawning_pool}, &dependencys.baneling_nest);
            depMap["hydralisk_den"] = depObj({&dependencys.lair}, &dependencys.hydralisk_den);
            depMap["infestation_pit"] = depObj({&dependencys.lair}, &dependencys.infestation_pit);
            depMap["hive"] = depObj({&dependencys.infestation_pit}, &dependencys.hive);
            depMap["nydus_network"] = depObj({&dependencys.lair}, &dependencys.nydus_network);
            depMap["ultralisk_cavern"] = depObj({&dependencys.hive}, &dependencys.ultralisk_cavern);
            depMap["greater_spire"] = depObj({&dependencys.hive}, &dependencys.greater_spine);
            depMap["spire"] = depObj({&dependencys.lair}, &dependencys.spire);

        }
        void initResources(){
            resMap["drone"] = resObj({rTU(&resources.drones, 1), rTU(&resources.supply, -1)});
            resMap["hatchery"] = resObj({rTU(&resources.hatchery, 1), rTU(&resources.bases, 1),
                 rTU(&resources.extractor_slots, 2), rTU(&resources.supply, 2)});
            resMap["spawning_pool"] = resObj({rTU(&resources.drones, -1),rTU(&resources.supply, 1)});
            resMap["zergling"] = resObj({rTU(&resources.zergling, 2), rTU(&resources.supply, -1)});
            resMap["overlord"] = resObj({rTU(&resources.overlord, 1), rTU(&resources.supply, 8)});
            resMap["overseer"] = resObj({rTU(&resources.overlord, -1)});
            resMap["hydralsik"] = resObj({rTU(&resources.supply, -2)});
            resMap["queen"] = resObj({rTU(&resources.supply, -2)});
            resMap["zergling"] = resObj({rTU(&resources.supply, -1), rTU(&resources.zergling, 2)});
            resMap["baneling"] = resObj({rTU(&resources.zergling, -1)});
            resMap["roach"] = resObj({rTU(&resources.supply, -2)});
            resMap["infestor"] = resObj({rTU(&resources.supply, -2)});
            resMap["nydus_worm"] = resObj();
            resMap["ultralisk"] = resObj({rTU(&resources.supply, -6)});
            resMap["brood_lord"] = resObj({rTU(&resources.supply, -2), rTU(&resources.corruptor, -1)});
            resMap["corruptor"] = resObj({rTU(&resources.corruptor, 1), rTU(&resources.supply, -2)});
            resMap["mutalisk"] = resObj({rTU(&resources.supply, -2)});
            //building:
            resMap["evolution_chamber"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["spore_crawler"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["lair"] = resObj({rTU(&resources.hatchery, -1), rTU(&resources.lair, 1)});
            resMap["spawning_pool"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["extractor"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1), rTU(&resources.extractor_slots, -1)});
            resMap["spine_crawler"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["roach_warren"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["baneling_nest"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["hydralisk_den"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["infestation_pit"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["hive"] = resObj({rTU(&resources.lair, -1)});
            resMap["nydus_network"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["ultralisk_cavern"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1)});
            resMap["spire"] = resObj({rTU(&resources.drones, -1), rTU(&resources.supply, 1), rTU(&resources.spire, 1)});
            resMap["greater_spire"] = resObj({rTU(&resources.spire, -1)});
        }
};