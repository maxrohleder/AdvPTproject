#include "Zerg_header.h"
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>


using namespace std;


class Zerg : public Zerg_header{

    private:
    //typedef 
    typedef bool (Zerg::*funcBool) (void);
    typedef void (Zerg::*funcVoid) (void);

    //struct for eventlist
    struct end_event{
        end_event(int i, funcVoid function) : end_time(i), func(function){}
        end_event(const end_event* e) : end_time(e->end_time), func(e->func){}
        ~end_event(){}
        int end_time;
        funcVoid func;
    };

    //global time
    int time = 1;

    //needed list structures
    list<funcBool> buildlist;
    list<end_event> eventlist;
    map<string, funcBool> buildmap;


    //helpers
    //helper for printlist
    void addToPrintlist(string type, string name){
        printlist.push_back(make_pair(type, name));
    }

    //helper for eventlist
    void addToEventlist(int t, funcVoid func){
        eventlist.push_front(end_event(time + t, func));
    }

    //helper for resource-check
    bool checkResources(int min, int supp = 0, int vesp = 0){
        if(minerals < min || vespene < vesp || (supply_max - supply_used) < supp ){
            return true;
        }
        return false;
    }
    
    
    //build and finish functions

    //drone

    bool droneBuild(){
        if (checkResources(5000, 1)){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 5000;
            ++supply_used;
            addToPrintlist("build-start", "drone");
            addToEventlist(17, &Zerg::droneFinish);
            return true;
        }
    }

    void droneFinish(){
        ++workers;
        distributeWorker();
        addToPrintlist("build-end", "drone");
    }

    //queen

    bool queenBuild(){
        if(checkResources(15000, 2) || spawning_pool < 1 || queen_slot < 1){
            return false;
        }else{
            minerals -= 15000;
            --queen_slot;
            supply_used += 2;
            addToPrintlist("build-start", "queen");
            addToEventlist(50, &Zerg::queenFinish);
            return true;
        }
    }

    void queenFinish(){
        ++queen;
        ++queen_slot;
        addToPrintlist("build-end", "queen");
    }



    //overlord

    bool overloredBuild(){
        if(checkResources(10000, 0)){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 10000;
            addToPrintlist("build-start", "overlord");
            addToEventlist(25, &Zerg::overlordFinish);
            return true;
        }
    }

    void overlordFinish(){
        ++overlord;
        supply_max += 8;
        addToPrintlist("build-end", "overlord");
    }

    //overseer

    bool overseerBuild(){
        if (checkResources(5000, 0, 5000) || overlord < 1 || lair_hive < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 5000;
            vespene -= 5000;
            --overlord;
            addToPrintlist("build-start", "overseer");
            addToEventlist(17, &Zerg::overseerFinish);
            return true;
        }
    }

    void overseerFinish(){
        ++overseer;
        addToPrintlist("build-end", "overseer");
    }

    //hydralisk

    bool hydraliskBuild() {
        if(checkResources(10000, 2, 5000) || hydralisk_den < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 10000;
            vespene -= 5000;
            supply_used += 2;
            addToPrintlist("build-start", "hydralisk");
            addToEventlist(33, &Zerg::hydraliskFinish);
            return true;
        }
    }

    void hydraliskFinish(){
        ++hydralisk;
        addToPrintlist("build-end", "hydralisk");
    }

    //zergling

    bool zerglingBuild(){
        if(checkResources(5000, 1) || spawning_pool < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 5000;
            supply_used += 1;
            addToPrintlist("build-start", "zergling");
            addToEventlist(24, &Zerg::zerglingFinish);
            return true;
        }
    }

    void zerglingFinish(){
        zergling += 2;
        addToPrintlist("build-end", "zergling");
    }

    //baneling

    bool banelingBuild(){
        if(checkResources(2500, 0, 2500) || zergling < 1 || baneling_nest < 1){
            return false;
        }else{
            minerals -= 2500;
            vespene -= 2500;
            --zergling;
            addToPrintlist("build-start", "baneling");
            addToEventlist(20, &Zerg::banelingFinish);
            return true;
        }
    }

    void banelingFinish(){
        ++baneling;
        addToPrintlist("build-end", "baneling");
    }

    //roach

    bool roachBuild(){
        if(checkResources(7500, 2, 2500) || roach_warren < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 7500;
            vespene -= 2500;
            supply_used += 2;
            addToPrintlist("build-start", "roach");
            addToEventlist(27, &Zerg::roachFinish);
            return true;
        }
    }

    void roachFinish(){
        ++roach;
        addToPrintlist("build-end", "roach");
    }

    //infestor

    bool infestorBuild(){
        if(checkResources(10000, 2, 15000) || infestation_pit < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 10000;
            vespene -= 15000;
            supply_used += 2;
            addToPrintlist("build-start", "infestor");
            addToEventlist(50, &Zerg::infestorFinish);
            return true;
        }
    }

    void infestorFinish(){
        ++infestor;
        addToPrintlist("build-end", "infestor");
    }

    //nydus_worm

    bool NydusWormBuild(){
        if(checkResources(10000, 0, 10000) || nydus_network < 1){
            return false;
        }else{
            minerals -= 10000;
            vespene -= 10000;
            addToPrintlist("build-start", "nydus_worm");
            addToEventlist(20, &Zerg::NydusWormFinish);
            return true;
        }
    }

    void NydusWormFinish(){
        ++nydus_worm;
        addToPrintlist("build-end", "nydus_worm");
    }

    //ultralisk

    bool ultraliskBuild(){
        if(checkResources(30000, 6, 20000) || ultralisk_cavern < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 30000;
            vespene -= 20000;
            supply_used += 6;
            addToPrintlist("build-start", "ultralisk");
            addToEventlist(55, &Zerg::ultraliskFinish);
            return true;
        }
    }

    void ultraliskFinish(){
        ++ultralisk;
        addToPrintlist("build-end", "ultralisk");
    }

    //brood_lord

    bool broodLordBuild(){
        if(checkResources(15000, 2, 15000) || greater_spire < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 15000;
            --corruptor;
            supply_used += 2;
            addToPrintlist("build-start", "brood_lord");
            addToEventlist(34, &Zerg::broodLordFinish);
            return true;
        }
    }

    void broodLordFinish(){
        ++brood_lord;
        addToPrintlist("build-end", "brood_lord");
    }

    //corruptor

    bool corruptorBuild(){
        if(checkResources(15000, 2, 10000) || spire_greater_spire < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 15000;
            vespene -= 10000;
            supply_used += 2;
            addToPrintlist("build-start", "corruptor");
            addToEventlist(40, &Zerg::corruptorFinish);
            return true;
        }
    }

    void corruptorFinish(){
        ++corruptor;
        addToPrintlist("build-end", "corruptor");
    }

    //mutalisk

    bool mutaliskBuild(){
        if(checkResources(10000, 2, 10000) || spire_greater_spire < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 10000;
            vespene -= 10000;
            supply_used += 2;
            addToPrintlist("build-start", "mutalisk");
            addToEventlist(33, &Zerg::mutaliskFinish);
            return true;
        }
    }

    void mutaliskFinish(){
        ++mutalisk;
        addToPrintlist("build-end", "mutalisk");
    }



    //structures

    //hatchary

    bool hatcharyBuild(){
        if(checkResources(30000)){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 30000;
            addToPrintlist("build-start", "hatchery");
            addToEventlist(100, &Zerg::hatcharyFinish);
            return true;
        }
    }

    void hatcharyFinish(){
        ++hatchary;
        ++bases;
        geyser_max += 2;
        supply_max += 2;
        ++queen_slot;
        addToPrintlist("build-end", "hatchary");
    }

    //evolution_chamber

    bool evolutionChamberBuild(){
        if(checkResources(7500)){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 7500;
            addToPrintlist("build-start", "evolution_chamber");
            addToEventlist(35, &Zerg::evolutionChamberFinish);
            return true;
        }
    }

    void evolutionChamberFinish(){
        ++evolution_chamber;
        addToPrintlist("build-end", "evolution_chamber");
    }

    //spore_crawler

    bool sporeCrawlerBuild(){
        if(checkResources(7500) || evolution_chamber < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 7500;
            addToPrintlist("build-start", "spore_crawler");
            addToEventlist(30, &Zerg::sporeCrawlerFinish);
            return true;
        }
    }

    void sporeCrawlerFinish(){
        ++spore_crawler;
        addToPrintlist("build-end", "spore_crawler");
    }

    //lair

    bool lairBuild(){
        if(checkResources(15000, 0, 10000) || spawning_pool < 1 || hatchary < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 10000;
            --hatchary;
            addToPrintlist("build-start", "lair");
            addToEventlist(80, &Zerg::lairFinish);
            return true;
        }
    }

    void lairFinish(){
        ++lair;
        ++lair_hive;
        addToPrintlist("build-end", "lair");
    }

    //spawning_pool

    bool spawningPoolBuild(){
        if(checkResources(20000)){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 20000;
            addToPrintlist("build-start", "spawning_pool");
            addToEventlist(65, &Zerg::spawningPoolFinish);
            return true;
        }
    }

    void spawningPoolFinish(){
        ++spawning_pool;
        addToPrintlist("build-end", "spawning_pool");
    }

    //extractor

    bool extractorBuild(){
        if(checkResources(2500) || geyser_max < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 2500;
            addToPrintlist("build-start", "extractor");
            addToEventlist(30, &Zerg::extractorFinish);
            return true;
        }
    }

    void extractorFinish(){
        ++extractor;
        workers_vesp_max += 3;
        addToPrintlist("build-end", "extractor");
    }

    //spine_crawler

    bool spineCrawlerBuild(){
        if(checkResources(10000) || spawning_pool < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 10000;
            addToPrintlist("build-start", "spine_crawler");
            addToEventlist(50, &Zerg::spineCrawlerFinish);
            return true;
        }
    }

    void spineCrawlerFinish(){
        ++spine_crawler;
        addToPrintlist("build-end", "spine_crawler");
    }

    //roach_warren

    bool roachWarrenBuild(){
        if(checkResources(15000) || spawning_pool < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 15000;
            addToPrintlist("build-start", "roach_warren");
            addToEventlist(55, &Zerg::roachWarrenFinish);
            return true;
        }
    }

    void roachWarrenFinish(){
        ++roach_warren;
        addToPrintlist("build-end", "roach_warren");
    }

    //baneling_nest

    bool banelingNestBuild(){
        if(checkResources(10000, 0, 5000) || spawning_pool < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 10000;
            vespene -= 5000;
            addToPrintlist("build-start", "baneling_nest");
            addToEventlist(50, &Zerg::banelingNestFinish);
            return true;
        }
    }

    void banelingNestFinish(){
        ++baneling_nest;
        addToPrintlist("build-end", "baneling_nest");
    }

    //hydralisk_den

    bool hydraliskDenBuild(){
        if(checkResources(10000, 0, 10000) || lair_hive < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 10000;
            vespene -= 10000;
            addToPrintlist("build-start", "hydralisk_ban");
            addToEventlist(40, &Zerg::hydraliskDenFinish);
            return true;
        }
    }

    void hydraliskDenFinish(){
        ++hydralisk_den;
        addToPrintlist("build-end", "hydralisk_den");
    }

    //infestation_pit

    bool infestationPitBuild(){
        if(checkResources(10000, 0, 10000) || lair_hive < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 10000;
            vespene -= 10000;
            addToPrintlist("build-start", "infestation_pit");
            addToEventlist(40, &Zerg::infestationPitFinish);
            return true;
        }
    }

    void infestationPitFinish(){
        ++infestation_pit;
        addToPrintlist("build-end", "infestation_pit");
    }

    //hive

    bool hiveBuild(){
        if(checkResources(20000, 0, 15000) || infestation_pit < 1 || lair < 1){
            return false;
        }else{
            minerals -= 20000;
            vespene -= 15000;
            --lair;
            addToPrintlist("build-start", "hive");
            addToEventlist(100, &Zerg::hiveFinish);
            return true;
        }
    }

    void hiveFinish(){
        ++hive;
        addToPrintlist("build-end", "hive");
    }

    //nydus_network

    bool nydusNetworkBuild(){
        if(checkResources(15000, 0, 20000) || lair_hive < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 15000;
            vespene -= 20000;
            addToPrintlist("build-start", "nydus_network");
            addToEventlist(50, &Zerg::nydusNetworkFinish);
            return true;
        }
    }

    void nydusNetworkFinish(){
        ++nydus_network;
        addToPrintlist("build-end", "nydus_network");
    }

    //ultralisk_cavern

    bool ultraliskCavernBuild(){
        if(checkResources(15000, 0, 20000) || hive < 1){
            return true;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 15000;
            vespene -= 20000;
            addToPrintlist("build-start", "ultralisk_cavern");
            addToEventlist(65, &Zerg::ultraliskCavernFinish);
            return true;
        }
    }

    void ultraliskCavernFinish(){
        ++ultralisk_cavern;
        addToPrintlist("build-end", "ultralisk_cavern");
    }

    //greater_spire

    bool greaterSpireBuild(){
        if(checkResources(10000, 0, 15000) || hive < 1 || spire < 1){
            return false;
        }else{
            minerals -= 10000;
            vespene -= 15000;
            --spire;
            addToPrintlist("build-start", "greater_spire");
            addToEventlist(100, &Zerg::greaterSpireFinish);
            return true;
        }
    }

    void greaterSpireFinish(){
        ++greater_spire;
        addToPrintlist("build-end", "greater_spire");
    }

    //spire

    bool spireBuild(){
        if(checkResources(20000, 0, 20000) || lair_hive < 1){
            return false;
        }else{
            if(!getWorker()){
                return false;
            }
            minerals -= 20000;
            vespene -= 20000;
            addToPrintlist("build-start", "spire");
            addToEventlist(100, &Zerg::spireFinish);
            return true;
        }
    }

    void spireFinish(){
        ++spire;
        ++spire_greater_spire;
        addToPrintlist("build-end", "spire");
    }


    //update functions
    

    void updateEventlist(){
        while(1){
            auto i = find_if(eventlist.begin(), eventlist.end(), [this](const end_event p){return p.end_time == time;});
            if(i == eventlist.end()){
                return;
            }else{
                (this->*(i->func))(); 
                eventlist.erase(i);
            }

        }
    }

    void updateBuildlist(){
        if((this->*(*buildlist.begin()))()){
            buildlist.pop_front();
        }
    }

    void buildBuildlist(string filename){
        ifstream file;
        string unit_name;
        file.open(filename);
        if(file.is_open()){
            while(getline(file, unit_name)){
                buildlist.push_back(buildmap[unit_name]);
            }
            file.close();
        }else{
            cerr << "could not read file" << endl;
            exit(-1);
        }
    }

    void buildBuildmap(){
        buildmap["drone"] = &Zerg::droneBuild;
        buildmap["overlord"] = &Zerg::overloredBuild;
        buildmap["overseer"] = &Zerg::overseerBuild;
        buildmap["hydralisk"] = &Zerg::hydraliskBuild;
        buildmap["queen"] = &Zerg::queenBuild;
        buildmap["zergling"] = &Zerg::zerglingBuild;
        buildmap["baneling"] = &Zerg::banelingBuild;
        buildmap["roach"] = &Zerg::roachBuild;
        buildmap["infestor"] = &Zerg::infestorBuild;
        buildmap["nydus_worm"] = &Zerg::NydusWormBuild;
        buildmap["ultralisk"] = &Zerg::ultraliskBuild;
        buildmap["brood_lord"] = &Zerg::broodLordBuild;
        buildmap["corruptor"] = &Zerg::corruptorBuild;
        buildmap["mutalisk"] = &Zerg::mutaliskBuild;
        buildmap["hatchary"] = &Zerg::hatcharyBuild;
        buildmap["evolution_chamber"] = &Zerg::evolutionChamberBuild;
        buildmap["spore_crawler"] = &Zerg::sporeCrawlerBuild;
        buildmap["lair"] = &Zerg::lairBuild;
        buildmap["spawning_pool"] = &Zerg::spawningPoolBuild;
        buildmap["extractor"] = &Zerg::extractorBuild;
        buildmap["spine_crawler"] = &Zerg::spineCrawlerBuild;
        buildmap["roach_warren"] = &Zerg::roachWarrenBuild;
        buildmap["baneling_nest"] = &Zerg::banelingNestBuild;
        buildmap["hydralisk_den"] = &Zerg::hydraliskDenBuild;
        buildmap["infestion_pit"] = &Zerg::infestationPitBuild;
        buildmap["hive"] = &Zerg::infestationPitBuild;
        buildmap["nydus_network"] = &Zerg::nydusNetworkBuild;
        buildmap["ultralisk_cavern"] = &Zerg::ultraliskCavernBuild;
        buildmap["greater_spire"] = &Zerg::greaterSpireBuild;
        buildmap["spire"] = &Zerg::spireBuild;
    }

    public:

    Zerg(const string filename){
        buildBuildmap();
        initLarvaelist();
        supply_max = 10;
        buildBuildlist(filename);
        printHeader(1);
    }
    Zerg(const Zerg& z){}
    ~Zerg(){}
    int run() {
        for(;time < 1000;++time){
            updateRecources();
            print(time);
            return 0;
        }
        return 1;
    }

    int runTest(int endTime) {
        for(;time < endTime;++time){
            updateRecources();
            updateEventlist();
            if(!buildlist.empty()){
                updateBuildlist();
            }
            if(!printlist.empty()){
                print(time);
                if(buildlist.empty() && eventlist.empty()){
                    cout << "\r\t\t}  " << endl;
                    printFinish();
                    return 0;
                }else{
                    cout << endl;
                }
            }
        }
        printFinish();
        return 1;
    }


};