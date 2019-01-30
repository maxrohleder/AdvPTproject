#pragma once
#include <string>
#include <list>
#include <map>
#include <iostream>
#include "./parser.h"
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include "../Validator/ValidatorZerg.h"
#include "global_enums.h"
#include "../Zerg.h"
#include "../Protoss.h"
#include "../Terran.h"

bool comp(const pair<list<string>, int>& first, const pair<list<string>, int>& second){
    return first.second < second.second;
}

//this will generate a buildlist out of digList for dependencies, once for units only needed once and multiple for units needed multiple times
class list_builder{
    public:
        list_builder(string target = "", const string path_to_techtree = "", int amount = 1, RaceType r = ZERG) : race_flag(r), target(target), amount(amount){
            init();
            p = par(path_to_techtree, false);
        }
        list_builder(const list_builder& lb) : p(lb.p), race_flag(lb.race_flag){
            init();
        }
        ~list_builder(){};

        list_builder operator=(const list_builder& lb){
            p = lb.p;
            used_only_once = lb.used_only_once;
            once = lb.once;
            multiple = lb.multiple;
            digList = lb.digList;
            vespene = lb.vespene;
            race_flag = lb.race_flag;
            buildList = lb.buildList;
            return *this;
        }


        
        void appendNLists(list<pair<list<string>, int>> &buildlists, int n, bool sort = false){
            for(int i = 0; i < n; i++){
                list<string> l = createListPush();
                runAndInsertList(buildlists, l);
            }
            if(sort){
                buildlists.sort(comp);
            }
        }

        vector<string>* getMultiple(){
            return &multiple;
        }


    protected:


    //TODO Raceflags
    void init(){
        srand(seed);
        if(race_flag == ZERG){
            used_only_once = used_only_once_zerg;
            initZerg();
        }else if(race_flag == TERRAN){
            used_only_once = used_only_once_terran;
            initTerran();
        }else{
            used_only_once = used_only_once_protoss;
            initProtoss();
        }
    }

    void initZerg(){
        if(target == "drone"){
            amount -= 6;
        }else if(target == "overlord" || target == "hatchery"){
            --amount;
        }
        multiple.push_back("drone");
        multiple.push_back("hatchery");
        multiple.push_back("overlord");
        once.push_back("extractor");
        once.push_back("extractor");
    }


    void initTerran(){
        multiple.push_back("scv");
        multiple.push_back("command_center");
        multiple.push_back("supply_depot");
        once.push_back("refinery");
        once.push_back("refinery");
    }

    void initProtoss(){
        multiple.push_back("probe");
        multiple.push_back("nexus");
        multiple.push_back("pylon");
        once.push_back("assimilator");
        once.push_back("assimilator");
    }

    //create a single list for push
    list<string> createListPush(){
        buildDigList(target);
        for(int i = 1; i < amount; ++i){
            digList.push_back(target);
        }
        while(!digList.empty()){
            string r = getRandomUnit();
            if(r != ""){
                addToBuildlist(r);
            }
        }
        list<string> ret = buildList;
        reset();
        return ret;
    }

    void runAndInsertList(list<pair<list<string>, int>>& buildlists, list<string> &bl){
        int time = MAX_TIME;
        bool valid = true;
        // TODO generalize to all races
        if(race_flag == ZERG){
            ZergChecker zc = ZergChecker();
            valid = zc.run(bl);
            if(valid){
                Zerg z(bl);
                time = z.getEndTime(5000);
            }
        }else if(race_flag == TERRAN){
            parser p (path_techtree_terran, bl, false);
            if(validate(p, false)){
                Terran t(bl);
                time = t.getEndTime(5000);
            }
        }else {
            // TODO resolve include issues and setup testing
        }
        buildlists.push_back(make_pair(bl, time));        
    }

    //add 2 vespene producers
    void addVespene(){
        if(race_flag == ZERG){
            once.push_back("extractor");
            once.push_back("extractor");
        }else if(race_flag == TERRAN){
            once.push_back("refinery");
            once.push_back("refinery");
        }else{
            once.push_back("assimilator");
            once.push_back("assimilator");
        }
    }

    void reset(){
        once.clear();
        multiple.clear();
        buildList.clear();
        digList.clear();
        vespene = false;
        if(race_flag == ZERG){
            initZerg();
        }else if(race_flag == TERRAN){
            initTerran();
        }else{
            initProtoss();
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
        if(find(digList.begin(), digList.end(), name) == digList.end()){
            digList.push_back(name);
        }
    }

    //add producable to either once or multiple
    void addToProducable(string name){
        if(find(used_only_once.begin(), used_only_once.end(), name) == used_only_once.end()){
            if(name == target){
                return;
            }else{
                multiple.push_back(name);
            }
        }
    }

    //helper to decide if already in once
    void addToOnce(string name){
        if(find(once.begin(), once.end(), name) == once.end()){
            once.push_back(name);
        }
    }

    void printBuildList(){
        cout << "buildList: ";
        for(auto i : buildList){
            cout << i << " ";
        }
        cout << endl;
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
        for(auto i : buildList){
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
        string r = *digList.begin();
        digList.pop_front();
        return r;
    }

    string getRandomUnit(){
        int list_to_use = rand() % prob_max;
        string new_unit;
        if(list_to_use < prob_mult){
            new_unit = getFromMultiple();
        }else if(list_to_use < prob_once){
            if(vespene){
                new_unit = getFromOnce();
            }else{
                return "";
            }
        }else{
            new_unit = getFromDigList();
            addToProducable(new_unit);
        }
        if(new_unit == "hatchery" || new_unit == "nexus" || new_unit == "command_center"){
            addVespene();
        }
        return new_unit;
    }

    void addToBuildlist(string name){
        buildList.push_back(name);
    }

    par p;
    string target;
    list<string> used_only_once_zerg = {"evolution_chamber",
         "spore_crawler", "spawning_pool", "spine_crawler", "roach_warren",
         "baneling_nest", "hydralisk_den", "infestation_pit", "nydus_network",
         "ultralisk_cavern", "greater_spire", "spire"};

    list<string> used_only_once_terran = {"engineering bay", "armory", "fusion_core", "ghost_academy"};

    // all buildings which are never listed as produced_by only dependencies...
    list<string> used_only_once_protoss = { "cybernetics_core","robotics_bay","twilight_council","templar_archives",
                                            "dark_shrine","fleet_beacon","forge"};


    list<string>& used_only_once = used_only_once_zerg;
    vector<string> once;
    vector<string> multiple;
    list<string> digList;
    bool vespene = false;
    RaceType race_flag;
    list<string> buildList;
    int amount = 1;

    //test for more valid lists
    //probabilitys:
    int prob_max = 30;
    int prob_mult = 20;
    int prob_once = prob_mult + 5;
    int prob_dig = prob_max - prob_mult - prob_once;
    

};