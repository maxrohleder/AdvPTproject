#include <iostream>
#include <list>
#include <string>
#include <algorithm>

#include "Opt.h"
#include "global_enums.h"

using namespace std;

const static list<string> protoss_units = {"probe","zealot","stalker","sentry","warp_prism","immortal","observer","colossus","high_templar",
                                    "dark_templar","mothership","phoenix","void_ray","carrier","nexus","pylon","gateway","cybernetics_core",
                                    "robotics_facility","robotics_bay","twilight_council","templar_archives","dark_shrine","stargate",
                                    "fleet_beacon","assimilator","forge","photon_cannon"};
const static list<string> zerg_units = {   "overlord","overseer","hydralisk","drone","queen","zergling","baneling","roach","infestor",
                                    "nydus_worm","ultralisk","brood_lord","corruptor","mutalisk","hatchery","evolution_chamber",
                                    "spore_crawler","lair","spawning_pool","extractor","spine_crawler","roach_warren","baneling_nest",
                                    "hydralisk_den","infestation_pit","hive","nydus_network","ultralisk_cavern","greater_spire","spire"};
const static list<string> terran_units = { "scv","marine","marauder","reaper","ghost","hellion","siege_tank","thor","medivac","viking",
                                    "raven","banshee","battlecruiser","command_center","orbital_command","planetary_fortress",
                                    "refinery","engineering_bay","missile_turret","sensor_tower","barracks","factory","armory",
                                    "bunker","ghost_academy","starport","fusion_core","supply_depot","barracks_with_reactor",
                                    "barracks_with_tech_lab","starport_with_reactor","starport_with_tech_lab","factory_with_reactor",
                                    "factory_with_tech_lab"};

int main(int argc, char *argv[]){
    if(argc < 4 || argc > 5){
        cout << "usage: " << argv[0] << " <rush/push> <unit> <seconds/amount> (optional) <from_bash>" << endl;
        exit(1);
    }

    // set path to techtrees if were calling this from bashscript
    if(argc == 5 && stoi(string(argv[4])) == 1){
        path_techtree_protoss = "Optimization/techtree_protoss.csv";
        path_techtree_terran = "Optimization/techtree_terran.csv";
        path_techtree_zerg = "Optimization/techtree_zerg.csv";
    }
    bool rush = (string(argv[1]) == "rush");
    string unit_to_build = string(argv[2]);
    int amount = stoi(string(argv[3]));

    if(find_if(protoss_units.begin(), protoss_units.end(), [unit_to_build](const string n){return n == unit_to_build;}) != protoss_units.end()){
        Opt O(RaceType::PROTOSS, string(path_techtree_protoss), unit_to_build, amount, rush);
        O.setHyper(100, 20, 100, 30);
        //tries out seeds
        O.find_best_hyper(10);
        //O.optimize_fake();       
        //O.printWinner();
        O.runWinner(); 
    }
    else if(find_if(zerg_units.begin(), zerg_units.end(), [unit_to_build](const string n){return n == unit_to_build;}) != zerg_units.end()){
        Opt O(RaceType::ZERG, path_techtree_zerg, unit_to_build, amount, rush);
        O.setHyper(100, 20, 100, 15); //100, 20, 100, 30       
        O.optimize();
        //O.optimize_fake();       
        //O.printWinner();
        O.runWinner();
        //O.printWinner(); //test purpose 
    }
    else if(find_if(terran_units.begin(), terran_units.end(), [unit_to_build](const string n){return n == unit_to_build;}) != terran_units.end()){
        Opt O(RaceType::TERRAN, string(path_techtree_terran), unit_to_build, amount, rush);
        O.setHyper(100, 20, 100, 30); // 100, 20, 100, 30
        O.optimize();
        //O.optimize_fake();       
        //O.printWinner();
        O.runWinner(); 
        //O.printWinner(); //test purpose
    }
    else{
        cout << "item not recognized!" << endl;
    }
    return 0;
}
