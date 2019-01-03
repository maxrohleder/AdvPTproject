#include <string>
#include <iostream>
#include <list>
#include <array>
#include <random>

using namespace std;

array<string, 30> zergList = {"drone", "overlord", "overseer", "hydralisk", "queen",
    "zergling", "baneling", "roach", "infestor", "nydus_worm", "ultralisk", "brood_lord",
    "corruptor", "mutalisk", "hatchery", "evolution_chamber", "spore_crawler",
    "lair", "spawning_pool", "extractor", "spine_crawler", "roach_warren", "baneling_nest",
    "hydralisk_den", "infestation_pit", "hive", "nydus_network", "ultralisk_cavern",
    "greater_spire", "spire"};

list<string> createRandomZergList(int length){
    list<string> retList;
    random_device generator;
    uniform_int_distribution<int> distribution(0, 29);
    for(int i = 0; i < length; ++i){
        int pos = distribution(generator);
        retList.push_back(zergList[pos]);
    }
    return retList;
}

