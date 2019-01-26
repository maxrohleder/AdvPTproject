#pragma once
#include <iostream>
#include <list>
#include <map>
#include <string>
#include "Validator.h"

using namespace std;

struct depTerran{
    int command_center = 1;
    int orbital_command = 0;
    int planetary_fortress = 0;
    //int refinery = 0;
    int engineering_bay = 0;
    int barracks = 0;
    int factory = 0;
    int armory = 0;
    int ghost_academy = 0;
    int starport = 0;
    int fusion_core = 0;
    int barracks_with_tech_lab = 0;
    int starport_with_tech_lab = 0;
    int factory_with_tech_lab = 0;
};

struct resTerran{
    int scv = 7;
    int supply = 11;
    int bases = 1;
    int command_center = 1;
    int orbital_command = 0;
    int barracks = 0;
    int factory = 0;
    int starport = 0;
};


class TerranChecker {

    public:
        TerranChecker(){
            initDependency();
            initResources();
            val = Validator(depMap, resMap);
        }
        TerranChecker(const TerranChecker* zc){}
        ~TerranChecker(){}

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
    depTerran dependencys;
    resTerran resources;
    map<string, depObj> depMap;
    map<string, resObj> resMap;
    Validator val;
    void initDependency(){

    }

    void initResources(){
        resMap["scv"] = resObj();
        resMap["marine"] = resObj(); 
        resMap["marauder"] = resObj();
        resMap["reaper"] = resObj();
        resMap["ghost"] = resObj();
        resMap["hellion"] = resObj();
        resMap["siege_tank"] = resObj();
        resMap["thor"] = resObj();
        resMap["medivac"] = resObj();
        resMap["viking"] = resObj();
        resMap["raven"] = resObj();
        resMap["banshee"] = resObj();
        resMap["battlecruiser"] = resObj();
        //buildings
        resMap["command_center"] = resObj();
        resMap["orbital_command"] = resObj();
        resMap["planetary_fortress"] = resObj();
        resMap["refinery"] = resObj();
        resMap["engineering_bay"] = resObj();
        resMap["missile_turret"] = resObj();
        resMap["sensor_tower"] = resObj();
        resMap["barracks"] = resObj();
        resMap["factory"] = resObj();
        resMap["armory"] = resObj();
        resMap["bunker"] = resObj();
        resMap["ghost_academy"] = resObj();
        resMap["starport"] = resObj();
        resMap["fusion_core"] = resObj();
        resMap["supply_depot"] = resObj();
        resMap["barracks_with_reactor"] = resObj();
        resMap["barracks_with_tech_lab"] = resObj();
        resMap["starport_with_reactor"] = resObj();
        resMap["starport_with_tech_lab"] = resObj();
        resMap["factory_with_reactor"] = resObj();
        resMap["factory_with_tech_lab"] = resObj();

    }

};