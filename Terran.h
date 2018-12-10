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
        finishInformation(int time, funcInt func, int build): finishTime(time), buildslot(build), function(func){
        }
        ~finishInformation(){}
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

    void addToPrintlist(string type, string name){
        printlist.push_back(make_pair(type, name));
    }

    void addToEventlist(int time, funcInt func, int build = NULL){
        finishInformation fin(time, func, build);
        eventlist.push_front(fin);
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
            addToPrintlist("build-start","scv");
            //finishInformation struct_scv(timestep + 17, NULL, &Terran::scvFinish);
            addToEventlist(timestep + 17, &Terran::scvFinish);
            //eventlist.push_front(struct_scv);
            return true;
        }
    }

    void scvFinish(int slot){
        ++command_center_buildslots; 
        ++workers;
        ++workers_minerals;
        addToPrintlist("build-end", "scv");
    }
    
    bool marineBuild(){
        if(minerals < 5000 || ( supply_max -supply_used ) < 1 || (barracks_buildslots < 1 && barracks_with_teck_lab_buildslots < 1 ) ){
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
            addToPrintlist("build-start","marine");
            addToEventlist(timestep + 25, &Terran::marineFinish, slot_variable);
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
        addToPrintlist("build-end", "marine");
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
            addToPrintlist("build-start", "hellion");
            addToEventlist(timestep + 30, &Terran::hellionFinish, slot_variable);
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
        addToPrintlist("build-end", "hellion");
    }

// ####################### end units ####################

/*
    Functions for buildings
*/

    bool commandCenterBuild(){
        if(minerals < 40000 || workers < 1){
            return false;
        }else{
            minerals -= 40000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "command_center");
            addToEventlist(timestep + 100, &Terran::commandCenterFinish);
            return true;
        }
    }

    void commandCenterFinish(int useless){
        ++workers;
        ++workers_minerals;
        supply_max += 11;
        addToPrintlist("build-end", "command_center");
    }

    //TODO energie eigenschaften hinzufügen
    bool orbitalCommandBuild(){
        if(minerals < 15000 || workers < 1 || command_center < 1 || barracks_total < 1 ){
            return false;
        }else{
            minerals -= 15000;
            addToPrintlist("build-start", "orbital_command");
            addToEventlist(timestep + 35, &Terran::orbitalCommandFinish);
            return true;
        }
    }

    void orbitalCommandFinish(int useless){
        ++orbital_command;
        --command_center;
        addToPrintlist("build-end","orbital_command");
    }

    bool planetaryFortressBuild(){
        if(minerals < 15000 || vespene < 15000 || command_center < 1 || engineering_bay < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 15000;
            addToPrintlist("build-start", "planetary_fortress");
            addToEventlist(timestep + 50, &Terran::planetaryFortressFinish);
            return true;
        }
    }

    void planetaryFortressFinish(int useless){
        ++planetary_fortress;
        --command_center;
        addToPrintlist("build-end", "planetary_fortress");
    }

    bool refineryBuild(){
        if(minerals < 7500 || workers < 1 || refinery + 1 > bases * geyser_max){
            return false;
        }else{
            minerals -= 7500;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start","refinery");
            addToEventlist(timestep + 30, &Terran::refineryFinish);
            return true;
        }
    }

    void refineryFinish(int useless){
        ++workers_vesp; //worker der vorher bei minerals abgezogen wurde wird jetzt vespene zugewiesen
        ++workers;
        ++refinery;
        addToPrintlist("build-end","refinery");
    }

    bool engineeringBayBuild(){
        if(minerals < 12500 || workers < 1){
            return false;
        }else{
            minerals -= 12500;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start","engineering_bay");
            addToEventlist(timestep + 35, &Terran::engineeringBayFinish);
            return true;
        }
    }

    void engineeringBayFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++engineering_bay;
        addToPrintlist("build-end","engineering_bay");
    }

    bool missileTurretBuild(){
        if(minerals < 10000 || workers < 1 || engineering_bay < 1){
            return false;
        }else{
            minerals -= 10000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "missile_turret");
            addToEventlist(timestep + 25, &Terran::missileTurretFinish);
            return true;
        }
    }

    void missileTurretFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++missile_turret;
        addToPrintlist("build-start", "missile_turret");
    }

    bool sensorTowerBuild(){
        if(minerals < 12500 || vespene < 10000 || workers < 1 || engineering_bay < 1){
            return false;
        }else{
            minerals -= 12500;
            vespene -= 10000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "sensor_tower");
            addToEventlist(timestep + 25, &Terran::sensorTowerFinish);
            return true;
        }
    }

    void sensorTowerFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++sensor_tower;
        addToPrintlist("build-start", "sensor_tower");
    }

    bool barracksBuild(){
        if(minerals < 15000 || supply_depot < 1 || workers < 1){
            return false;
        }else{
            minerals -= 15000;
            --workers;
            --workers_minerals; //TODO Vllt im wechsel mit Vesp worker?
            addToPrintlist("build-start", "barracks");
            addToEventlist(timestep + 65, &Terran::barracksFinish);
            return true;
        }
    }

    void barracksFinish(int useless){
        ++barracks;
        ++barracks_total;
        ++barracks_buildslots; 
        ++workers_minerals;
        ++workers;
        addToPrintlist("build-end", "barracks");
    }

    //TODO während upgrade buildslot gesperrt????
    bool barracksWithReactorBuild(){
        if(minerals < 5000 || vespene < 5000 || barracks < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 5000;
            addToPrintlist("build-start", "barracks_with_reactor");
            addToEventlist(timestep + 50, &Terran::barracksWithReactorFinish);
            return true;
        }
    }

    void barracksWithReactorFinish(int useless){
        ++barracks_with_reactor;
        --barracks;
        ++barracks_buildslots;
        addToPrintlist("build-end", "barracks_with_teck_lab");
    }

    bool barracksWithTeckLabBuild(){
        if(minerals < 5000 || vespene < 2500 || barracks < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 2500;
            addToPrintlist("build-start", "barracks_with_teck_lab");
            addToEventlist(timestep + 25, &Terran::barracksWithTeckLabFinish);
            return true;
        }
    }

    void barracksWithTeckLabFinish(int useless){
        ++barracks_with_teck_lab;
        --barracks;
        --barracks_buildslots;
        ++barracks_with_teck_lab_buildslots;
        addToPrintlist("build-end", "barracks_with_teck_lab");
    }

    bool factoryBuild(){
        if(minerals < 15000 || vespene < 10000 || workers < 1 || barracks_total < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 10000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "factory");
            addToEventlist(timestep + 60, &Terran::factoryFinish);
            return true;
        }
    }

    void factoryFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++factory_total;
        ++factory;
        ++factory_buildslots;
        addToPrintlist("build-end", "factory");
    }

    //TODO buildslot sperren????
    bool factoryWithReactorBuild(){
        if(minerals < 5000 || vespene < 5000 || factory < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 5000;
            addToPrintlist("build-start", "factory_with_reactor");
            addToEventlist(timestep + 50, &Terran::factoryWithReactorFinish);
            return true;
        }
    }

    void factoryWithReactorFinish(int useless){
        ++factory_with_reactor;
        --factory;
        ++factory_buildslots;
        addToPrintlist("build-end", "factory_with_reactor");
    }    

    bool factoryWithTeckLabBuild(){
        if(minerals < 5000 || vespene < 2500 || factory < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 2500;
            addToPrintlist("build-start", "factory_with_teck_lab");
            addToEventlist(timestep + 50, &Terran::factoryWithTeckLabFinish);
            return true;
        }
    }

    void factoryWithTeckLabFinish(int useless){
        ++factory_with_tech_lab;
        --factory;
        --factory_buildslots;
        ++factory_with_teck_lab_buildslots;
        addToPrintlist("build-end", "factory_with_teck_lab");
    }       

    bool armoryBuild(){
        if(minerals < 15000 || vespene < 10000 || workers < 1 || factory_total < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 10000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "armory");
            addToEventlist(timestep + 65, &Terran::armoryFinish);
            return true;
        }
    }

    void armoryFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++armory;
        addToPrintlist("build-end", "armory");
    }

    bool bunkerBuild(){
        if(minerals < 10000 || workers < 1 || barracks_total < 1){
            return false;
        }else{
            minerals -= 10000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "bunker");
            addToEventlist(timestep + 35, &Terran::bunkerFinish);
            return true;
        }
    }

    void bunkerFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++bunker;
        addToPrintlist("build-end", "bunker");
    }

    bool ghostAcademyBuild(){
        if(minerals < 15000 || vespene < 5000 || workers < 1 || barracks_total < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 5000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "ghost_academy");
            addToEventlist(timestep + 40, &Terran::ghostAcademyFinish);
            return true;
        }
    }

    void ghostAcademyFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++ghost_academy;
        addToPrintlist("build-end", "armory");
    }

    bool starportBuild(){
        if(minerals < 15000 || vespene < 10000 || workers < 1 || factory_total < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 10000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "starport");
            addToEventlist(timestep + 50, &Terran::starportFinish);
            return true;
        }
    }

    void starportFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++starport;
        ++starport_total;
        ++starport_buildslots;
        addToPrintlist("build-end", "starport");
    }    

    //TODO während upgrade buildslot gesperrt????
    bool starportWithReactorBuild(){
        if(minerals < 5000 || vespene < 5000 || factory < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 5000;
            addToPrintlist("build-start", "starport_with_reactor");
            addToEventlist(timestep + 50, &Terran::starportWithReactorFinish);
            return true;
        }
    }

    void starportWithReactorFinish(int useless){
        ++starport_with_reactor;
        --starport;
        ++starport_buildslots;
        addToPrintlist("build-end", "starport_with_reactor");
    }

    bool starportWithTeckLabBuild(){
        if(minerals < 5000 || vespene < 2500 || factory < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 2500;
            addToPrintlist("build-start", "factory_with_teck_lab");
            addToEventlist(timestep + 25, &Terran::starportWithTeckLabFinish);
            return true;
        }
    }

    void starportWithTeckLabFinish(int useless){
        ++factory_with_tech_lab;
        --factory;
        --factory_buildslots;
        ++factory_with_teck_lab_buildslots;
        addToPrintlist("build-end", "factory_with_teck_lab");
    }

    bool fusionCoreBuild(){
        if(minerals < 15000 || vespene < 15000 || workers < 1 || starport_total < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 10000;
            --workers;
            --workers_minerals;
            addToPrintlist("build-start", "fusion_core");
            addToEventlist(timestep + 65, &Terran::fusionCoreFinish);
            return true;
        }
    }

    void fusionCoreFinish(int useless){
        ++workers;
        ++workers_minerals;
        ++fusion_core;
        addToPrintlist("build-end", "fusion_core");
    }    

    bool supplyDepotBuild(){
        if(minerals < 10000 || workers < 1){
            return false;
        }else{
            minerals -= 10000;
            --workers_minerals;
            --workers;
            addToPrintlist("build-start", "supply_depot");
            addToEventlist(timestep + 30, &Terran::supplyDepotFinish);
            return true;
        }
    }

    void supplyDepotFinish(int useless){
        ++workers_minerals;
        ++workers;
        ++supply_depot;
        supply_max += 8;
        addToPrintlist("build-end", "supply_depot");
    }

//##################### end buildings #############################


public:

    Terran(const string filename){
        supply_max = 11;
        buildBuildmap();
        buildBuildlist(filename);
        printHeader(1);
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

    int testRun(int endTime) {
        for(;timestep < endTime;++timestep){
            updateResources();
            updateEventlist();
            if(!buildlist.empty()){
                updateBuildlist();
            }
            if(!printlist.empty()){
                print(timestep);
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