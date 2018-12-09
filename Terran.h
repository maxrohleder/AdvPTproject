#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>

#include "Terran_header.h"

using namespace std;

class Terran : public Terran_header{

private:
    int timestep = 1;

    typedef bool (Terran::*funcBool) (void);
    typedef void (Terran::*funcVoid) (void);

    // struct test{ //TODO in eventlist einfügen
    //     funcBool function;
    //     int buildslot;
    // }test;


    

    list<funcBool> buildlist;
    list<pair<int, funcVoid>> eventlist;
    map<string, funcBool> buildmap;

    void buildBuildmap(){
        buildmap["scv"] = &Terran::scvBuild; 
        buildmap["marine"] = &Terran::marineBuild;
        buildmap["hellion"] = &Terran::hellionBuild;

        buildmap["supply_depot"] = &Terran::supplyDepotBuild;
        buildmap["barracks"] = &Terran::barracksBuild;
        buildmap["factory"] = &Terran::factoryBuild;
        buildmap["refinery"] = &Terran::refineryBuild;
        buildmap["engineering_bay"] = &Terran::engineeringBayBuild;
    }

    void updateResources(){
        minerals += workers_minerals * minerals_rate;
        vespene += workers_vesp * vesp_rate;
    }

    void updateEventlist(){
        while(1){
            auto i = find_if(eventlist.begin(), eventlist.end(), [this](const pair<int, funcVoid> p){return p.first == timestep;});
            if(i == eventlist.end()){
                return;
            }else{
                (this->*(i->second))();
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

    bool scvBuild(){
        if(minerals < 5000 || ( supply_max - supply_used ) < 1 || command_center_buildslots < 1 ){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            --command_center_buildslots; 
            printlist.push_back(make_pair("build-start","scv"));
            eventlist.push_front(make_pair(timestep + 17, &Terran::scvFinish));
            return true;
        }
    }

    void scvFinish(){
        ++command_center_buildslots; 
        ++workers;
        ++workers_minerals;
        printlist.push_back(make_pair("build-end", "scv"));
    }
    
    bool marineBuild(){
        if(minerals < 50000 || ( supply_max -supply_used ) < 1 || (barracks_buildslots < 1 && barracks_with_teck_lab_buildslots < 1 ) ){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            // checks normal barrack first, if full then into barrack with teck lab
            if(barracks_buildslots > 0){
                --barracks_buildslots; 
                //test.function = &Terran::marineFinish;
                //test.buildslot = 0;
            }else{  
                --barracks_with_teck_lab_buildslots;
            }
            printlist.push_back(make_pair("build-start","marine"));
            eventlist.push_front(make_pair(timestep + 25, &Terran::marineFinish)); 
            return true; 
        }
    }

    void marineFinish(){
        //TODO wie weiß ich ob ich standard buildslot freigeben muss oder einen speziellen?
        ++marines;
        ++barracks_buildslots;
        printlist.push_back(make_pair("build-end", "marine"));
    }

    bool hellionBuild(){
        if(minerals < 10000 || (supply_max - supply_used) < 2 || (factory_buildslots < 1 && factory_with_teck_lab_buildslots < 1)){
            return false;
        }else{
            minerals -= 10000;
            if(factory_buildslots > 0){
                --factory_buildslots; 
                //test.function = &Terran::marineFinish;
                //test.buildslot = 0;
            }else{  
                --factory_with_teck_lab_buildslots;
            }
            printlist.push_back(make_pair("build-start", "hellion"));
            eventlist.push_front(make_pair(timestep + 30, &Terran::hellionFinish));
            return true;
        }
    }

    void hellionFinish(){
        ++hellion;
        //TODO wie weiß ich ob ich standard buildslot freigeben muss oder einen speziellen?
        ++factory_buildslots;
        printlist.push_back(make_pair("build-end", "hellion"));
    }



/*
    Functions for buildings
*/

    bool barracksBuild(){
        if(minerals < 15000 || supply_depot < 1 || workers < 1){
            return false;
        }else{
            minerals -= 15000;
            --workers;
            --workers_minerals; //TODO Vllt im wechsel mit Vesp worker?
            printlist.push_back(make_pair("build-start", "barracks"));
            eventlist.push_front(make_pair(timestep + 65, &Terran::barracksFinish));
            return true;
        }
    }

    void barracksFinish(){
        ++barracks;
        ++barracks_buildslots; 
        ++workers_minerals;
        ++workers;
        printlist.push_back(make_pair("build-end", "barracks"));
    }

    bool supplyDepotBuild(){
        if(minerals < 10000 || workers < 1){
            return false;
        }else{
            minerals -= 10000;
            --workers_minerals;
            --workers;
            printlist.push_back(make_pair("build-start", "supply_depot"));
            eventlist.push_front(make_pair(timestep + 30, &Terran::supplyDepotFinish));
            return true;
        }
    }

    void supplyDepotFinish(){
        ++workers_minerals;
        ++workers;
        ++supply_depot;
        supply_max += 8;
        printlist.push_back(make_pair("build-end", "supply_depot"));
    }

    bool refineryBuild(){
        if(minerals < 7500 || workers < 1 || refinery + 1 > bases * geyser_max){
            return false;
        }else{
            minerals -= 7500;
            --workers;
            --workers_minerals;
            printlist.push_back(make_pair("build-start","refinery"));
            eventlist.push_front(make_pair(timestep + 30, &Terran::refineryFinish));
            return true;
        }
    }

    void refineryFinish(){
        ++workers_vesp; //worker der vorher bei minerals abgezogen wurde wird jetzt vespene zugewiesen
        ++workers;
        ++refinery;
        printlist.push_back(make_pair("build-end","refinery"));
    }

    bool engineeringBayBuild(){
        if(minerals < 12500 || workers < 1){
            return false;
        }else{
            minerals -= 12500;
            --workers;
            --workers_minerals;
            printlist.push_back(make_pair("build-start","engineering_bay"));
            eventlist.push_front(make_pair(timestep + 35, &Terran::engineeringBayFinish));
            return true;
        }
    }

    void engineeringBayFinish(){
        ++workers;
        ++workers_minerals;
        ++engineering_bay;
        printlist.push_back(make_pair("build-end","engineering_bay"));
    }

    bool factoryBuild(){
        if(minerals < 15000 || vespene < 10000 || workers < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 10000;
            --workers;
            --workers_minerals;
            printlist.push_back(make_pair("build-start", "factory"));
            eventlist.push_front(make_pair(timestep + 60, &Terran::factoryFinish));
            return true;
        }
    }

    void factoryFinish(){
        ++workers;
        ++workers_minerals;
        ++factory;
        printlist.push_back(make_pair("build-end", "factory"));
    }


public:

    Terran(const string filename){
        supply_max = 11;
        buildBuildmap();
        buildBuildlist(filename);
    }

    ~Terran() {}


    int run (){
        for(; timestep < 10000; ++timestep){
            updateResources();
            updateEventlist();
            if(!buildlist.empty()){
                updateBuildlist(); 
            }
            if(!printlist.empty()){
                print(timestep);
            }

            if(buildlist.empty() && eventlist.empty()){
                return 0; 
            }
        }
        return 1;
    }

    int testRun (int time){
        //buildlist.push_back(&Terran::supplyDepotBuild);
        //buildlist.push_back(&Terran::marineBuild); 
        for(; timestep < time; ++timestep){
            updateResources();
            updateEventlist();
            if(!buildlist.empty()){
                updateBuildlist(); 
            }
            if(!printlist.empty()){
                print(timestep);
            }
            if(buildlist.empty() && eventlist.empty()){
                return 0; 
            }
        }
        return 1;
    }

};