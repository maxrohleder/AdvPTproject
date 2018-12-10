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
    typedef void (Terran::*funcInt) (int);

    struct finishInformation{
        int finishTime;
        int buildslot;
        funcInt function;
        finishInformation(int time, int build, funcInt func): finishTime(time), buildslot(build), function(func){

        }
    };

/*
    lists and map
*/

    list<funcBool> buildlist;
    list<finishInformation> eventlist;
    map<string, funcBool> buildmap;

    void buildBuildmap(){
        buildmap["scv"] = &Terran::scvBuild; 
        buildmap["marine"] = &Terran::marineBuild;
        buildmap["hellion"] = &Terran::hellionBuild;

        buildmap["orbital_command"] = &Terran::orbitalCommandBuild;
        buildmap["supply_depot"] = &Terran::supplyDepotBuild;
        buildmap["barracks"] = &Terran::barracksBuild;
        buildmap["factory"] = &Terran::factoryBuild;
        buildmap["refinery"] = &Terran::refineryBuild;
        buildmap["engineering_bay"] = &Terran::engineeringBayBuild;
    } 

    void updateEventlist(){
        while(1){
            auto i = find_if(eventlist.begin(), eventlist.end(), [this](const finishInformation p){return p.finishTime == timestep;});
            if(i == eventlist.end()){
                return;
            }else{
                (this->*((*i).function))((*i).buildslot);
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

//###############################end lists##################################

    void updateResources(){
        minerals += workers_minerals * minerals_rate;
        vespene += workers_vesp * vesp_rate;
    }

/*
    units
*/

    bool scvBuild(){
        if(minerals < 5000 || ( supply_max - supply_used ) < 1 || command_center_buildslots < 1 ){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            --command_center_buildslots; 
            printlist.push_back(make_pair("build-start","scv"));
            finishInformation struct_scv(timestep + 17, NULL, &Terran::scvFinish);
            eventlist.push_front(struct_scv);
            return true;
        }
    }

    void scvFinish(int slot){
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
            int slot_variable = 0;
            // checks normal barrack first, if full then into barrack with teck lab
            if(barracks_buildslots > 0){
                --barracks_buildslots;
            }else{  
                --barracks_with_teck_lab_buildslots;
                slot_variable = 1;
            }
            printlist.push_back(make_pair("build-start","marine"));
            finishInformation struct_marine(timestep + 25, slot_variable, &Terran::marineFinish);
            eventlist.push_front(struct_marine); 
            return true; 
        }
    }

    void marineFinish(int slot){
        if(slot == 0){
            ++barracks_buildslots;
        }else{
            ++barracks_with_teck_lab_buildslots;
        }
        ++marines;
        printlist.push_back(make_pair("build-end", "marine"));
    }

    bool hellionBuild(){
        if(minerals < 10000 || (supply_max - supply_used) < 2 || (factory_buildslots < 1 && factory_with_teck_lab_buildslots < 1)){
            return false;
        }else{
            minerals -= 10000;
            int slot_variable = 0;
            if(factory_buildslots > 0){
                --factory_buildslots;
            }else{  
                --factory_with_teck_lab_buildslots;
                slot_variable = 1;
            }
            printlist.push_back(make_pair("build-start", "hellion"));
            finishInformation struct_hellion(timestep + 30, slot_variable, &Terran::hellionFinish);
            eventlist.push_front(struct_hellion);
            return true;
        }
    }

    void hellionFinish(int slot){
        ++hellion;
        if(slot == 0){
            ++factory_buildslots;;
        }else{
            ++factory_with_teck_lab_buildslots;
        }
        printlist.push_back(make_pair("build-end", "hellion"));
    }

// ####################### end units ####################

/*
    Functions for buildings
*/

    //TODO energie eigenschaften hinzufügen
    bool orbitalCommandBuild(){
        if(minerals < 15000 || workers < 1 || command_center < 1 || (barracks < 1 && barracks_with_reactor < 1 && barracks_with_teck_lab < 1) ){
            return false;
        }else{
            minerals -= 15000;
            printlist.push_back(make_pair("build-start", "orbital_command"));
            finishInformation struct_orbital_command(timestep + 35, NULL, &Terran::orbitalCommandFinish);
            eventlist.push_front(struct_orbital_command);
            return true;
        }
    }

    void orbitalCommandFinish(int useless){
        ++orbital_command;
        --command_center;
        printlist.push_back(make_pair("build-end","orbital_command"));
    }

    bool barracksBuild(){
        if(minerals < 15000 || supply_depot < 1 || workers < 1){
            return false;
        }else{
            minerals -= 15000;
            --workers;
            --workers_minerals; //TODO Vllt im wechsel mit Vesp worker?
            printlist.push_back(make_pair("build-start", "barracks"));
            finishInformation struct_barracks(timestep + 65, NULL, &Terran::barracksFinish);
            eventlist.push_front(struct_barracks);
            return true;
        }
    }

    void barracksFinish(int useless){
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
            finishInformation struct_supply_depot(timestep + 30, NULL, &Terran::supplyDepotFinish);
            eventlist.push_front(struct_supply_depot);
            return true;
        }
    }

    void supplyDepotFinish(int useless){
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
            finishInformation struct_refinery(timestep + 30, NULL, &Terran::refineryFinish);
            eventlist.push_front(struct_refinery);
            return true;
        }
    }

    void refineryFinish(int useless){
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
            finishInformation struct_engineering_bay(timestep + 35, NULL, &Terran::engineeringBayFinish);
            eventlist.push_front(struct_engineering_bay);
            return true;
        }
    }

    void engineeringBayFinish(int useless){
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
            finishInformation struct_factory(timestep + 60, NULL, &Terran::factoryFinish);
            eventlist.push_front(struct_factory);
            return true;
        }
    }

    void factoryFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++factory;
        ++factory_buildslots;
        printlist.push_back(make_pair("build-end", "factory"));
    }

//##################### end buildings #############################


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