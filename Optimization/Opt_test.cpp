#include <string>
#include "Opt.h"
#include <iostream>
#include "global_enums.h"
#include <fstream>

using namespace std;

string path_protoss = "../../Optimization/techtree_protoss.csv";
string path_terran = "../../Optimization/techtree_terran.csv";
string path_zerg = "../../Optimization/techtree_zerg.csv";


int main(int argc, char* argv[]){

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

    const static list<string> test_zerg = {"zergling", "roach"};

    if(argc != 3){
        cout << "usage: " << argv[0] << " <number to push> <race> ((1): Protoss, (2): Zerg, (3): Terran, (99): DegubZerg)" << endl;
        exit(1);
    }
    int race = stoi(string(argv[2]));
    string number = string(argv[1]);
    switch (race)
    {
        case 1:
            for(auto i : protoss_units){
                ofstream out(i + number + ".txt");
                streambuf *coutbuf = std::cout.rdbuf();
                cout.rdbuf(out.rdbuf());
                Opt o(RaceType::PROTOSS, path_protoss, i, 10, false);
                o.optimize();
                o.runWinner();
                cout.rdbuf(coutbuf);
                out.close();
                cout << i << endl;
            }
            break;
        case 2:
            for(auto i : zerg_units){
                ofstream out(i + number + ".txt");
                streambuf *coutbuf = std::cout.rdbuf();
                cout.rdbuf(out.rdbuf());
                Opt o(RaceType::ZERG, path_zerg, i, 10, false);
                o.optimize();
                o.runWinner();
                cout.rdbuf(coutbuf);
                out.close();
                cout << i << endl;
            }
            break;
        case 3:
            for(auto i : terran_units){
                ofstream out(i + number + ".txt");
                streambuf *coutbuf = std::cout.rdbuf();
                cout.rdbuf(out.rdbuf());
                Opt o(RaceType::TERRAN, path_terran, i, 10, false);
                o.optimize();
                o.runWinner();
                cout.rdbuf(coutbuf);
                out.close();
                cout << i << endl;
            }
            break;
        case 99:
            for(auto i : test_zerg){
                ofstream out(i + number + ".txt");
                streambuf *coutbuf = std::cout.rdbuf();
                cout.rdbuf(out.rdbuf());
                Opt o(RaceType::ZERG, path_zerg, i, 10, false);
                o.optimize();
                o.runWinner();
                cout.rdbuf(coutbuf);
                out.close();
                cout << i << endl;
            }
            break;
        default:
            cout << "no case" << endl;
            break;
    }
    return 0;
}

