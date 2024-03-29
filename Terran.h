#pragma once
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
    
    struct orbital_command_id{
        orbital_command_id(string i): energy(make_pair(500000, 2000000)), id(i){}
        orbital_command_id(const orbital_command_id& i) : energy(i.energy), id(i.id){}
        ~orbital_command_id(){}
        void updateEnergy(){
            energy.first += 5625;
            if(energy.first > energy.second){
                energy.first = energy.second;
            }
        }
        bool triggerMule(Terran* t){
            if(energy.first < 500000){
                return false;
            }else{
                (t->mule_worker) += 4;
                energy.first -= 500000;
                t->addMuleToEventlist(t->timestep + 90, &Terran::muleFinish);
                t->addToPrintlist("special", "mule", id);
                return true;
            }
        }
        pair<int, int> energy;
        string id;
    };

    list<orbital_command_id> orbital_command_list;

    list<pair<string, bool>> barracks_names_list;
    list<pair<string, bool>> barracks_with_tech_lab_names_list;
    list<pair<string, bool>> barracks_upgrade_techlab;
    list<pair<string, bool>> barracks_upgrade_reactor;

    list<pair<string, bool>> factory_names_list;
    list<pair<string, bool>> factory_with_tech_lab_names_list;
    list<pair<string, bool>> factory_upgrade_techlab;
    list<pair<string, bool>> factory_upgrade_reactor;

    list<pair<string, bool>> starport_names_list;
    list<pair<string, bool>> starport_with_tech_lab_names_list;
    list<pair<string, bool>> starport_upgrade_techlab;
    list<pair<string, bool>> starport_upgrade_reactor;

    
    list<pair<string, bool>> marine_id_list;
    list<pair<string, bool>> marauder_id_list;
    list<pair<string, bool>> reaper_id_list;
    list<pair<string, bool>> ghost_id_list;
    list<pair<string, bool>> hellion_id_list;
    list<pair<string, bool>> siege_tank_id_list;
    list<pair<string, bool>> thor_id_list;
    list<pair<string, bool>> medivac_id_list;
    list<pair<string, bool>> viking_id_list;
    list<pair<string, bool>> raven_id_list;
    list<pair<string, bool>> banshee_id_list;
    list<pair<string, bool>> battlecruiser_id_list;



    string getProducerID(int building, list<pair<string,bool>>& get_list, list<pair<string,bool>>& put_list){
        // i = 0  => barracks i = 1 => factory, i = 2 => starport
        if(building == 0){
            pair<string, bool> producer_id = *get_list.begin();
            get_list.pop_front();
            put_list.push_back(producer_id);
            return producer_id.first;
        }else if(building == 1){
            pair<string, bool> producer_id = *get_list.begin();
            get_list.pop_front();
            put_list.push_back(producer_id);
            return producer_id.first;
        }else if(building == 2){
            pair<string, bool> producer_id = *get_list.begin();
            get_list.pop_front();
            put_list.push_back(producer_id);
            return producer_id.first;
        }
        return "";
    }

    void addToNamesList(int building, string s, bool b){
        // i = 0  => barracks i = 1 => factory, i = 2 => starport
        if(building == 0){
            barracks_names_list.push_back(make_pair(s, b));
        }else if(building == 1){
            factory_names_list.push_back(make_pair(s, b));
        }else if(building == 2){
            starport_names_list.push_back(make_pair(s, b));
        }
    }

    string upgradeToReactorStart(int building){
        // i = 0  => barracks i = 1 => factory, i = 2 => starport
        if(building == 0){
            for(list<pair<string, bool>>::iterator i = barracks_names_list.begin(); i != barracks_names_list.end(); ++i){
                if(!i->second){
                    string producer_id = i->first;
                    barracks_upgrade_reactor.push_back(*i);
                    barracks_names_list.erase(i);
                    return producer_id;
                }
            }
        }else if(building == 1){
            for(list<pair<string, bool>>::iterator i = factory_names_list.begin(); i != factory_names_list.end(); ++i){
                if(!i->second){
                    string producer_id = i->first;
                    factory_upgrade_reactor.push_back(*i);
                    factory_names_list.erase(i);
                    return producer_id;
                }
            }
        }else if(building == 2){
            for(list<pair<string, bool>>::iterator i = starport_names_list.begin(); i != starport_names_list.end(); ++i){
                if(!i->second){
                    string producer_id = i->first;
                    starport_upgrade_reactor.push_back(*i);
                    starport_names_list.erase(i);
                    return producer_id;
                }
            }
        }
        return "";
    }
    
    string upgradeToReactorFinish(int building){
        // i = 0  => barracks i = 1 => factory, i = 2 => starport
        if(building == 0){
            auto i = barracks_upgrade_reactor.begin();
            string producer_id = i->first;
            i->first = "barracks_with_reactor_" + to_string(barracks_with_reactor);
            i->second = true;
            barracks_names_list.push_back(*i);
            barracks_names_list.push_back(*i);
            barracks_upgrade_reactor.pop_front();
            return producer_id;
        }else if(building == 1){
            auto i = factory_upgrade_reactor.begin();
            string producer_id = i->first;
            i->first = "factory_with_reactor_" + to_string(factory_with_reactor);
            i->second = true;
            factory_names_list.push_back(*i);
            factory_names_list.push_back(*i);
            factory_upgrade_reactor.pop_front();
            return producer_id;
        }else if(building == 2){
            auto i = starport_upgrade_reactor.begin();
            string producer_id = i->first;
            i->first = "starport_with_reactor_" + to_string(starport_with_reactor);
            i->second = true;
            starport_names_list.push_back(*i);
            starport_names_list.push_back(*i);
            starport_upgrade_reactor.pop_front();
            return producer_id;
        }
        return "";
    }

    string upgradeToTechLabStart(int building){
        // i = 0  => barracks i = 1 => factory, i = 2 => starport
        if(building == 0){
            for(list<pair<string, bool>>::iterator i = barracks_names_list.begin(); i != barracks_names_list.end(); ++i){
                if(!i->second){
                    string producer_id = i->first;
                    barracks_upgrade_techlab.push_back(*i);
                    barracks_names_list.erase(i);
                    return producer_id;
                }
            }
        }else if(building == 1){
            for(list<pair<string, bool>>::iterator i = factory_names_list.begin(); i != factory_names_list.end(); ++i){
                if(!i->second){
                    string producer_id = i->first;
                    factory_upgrade_techlab.push_back(*i);
                    factory_names_list.erase(i);
                    return producer_id;
                }
            }
        }else if(building == 2){
            for(list<pair<string, bool>>::iterator i = starport_names_list.begin(); i != starport_names_list.end(); ++i){
                if(!i->second){
                    string producer_id = i->first;
                    starport_upgrade_techlab.push_back(*i);
                    starport_names_list.erase(i);
                    return producer_id;
                }
            }
        }
        return "";
    }

    string upgradeToTechLabFinish(int building){
        // i = 0  => barracks i = 1 => factory, i = 2 => starport
        if(building == 0){
            auto i = barracks_upgrade_techlab.begin();
            string producer_id = i->first;
            i->first = "barracks_with_tech_lab_" + to_string(barracks_with_tech_lab);
            barracks_with_tech_lab_names_list.push_back(*i);
            barracks_upgrade_techlab.pop_front();
            return producer_id;
        }else if(building == 1){
            auto i = factory_upgrade_techlab.begin();
            string producer_id = i->first;
            i->first = "factory_with_tech_lab_" + to_string(factory_with_tech_lab);
            factory_with_tech_lab_names_list.push_back(*i);
            factory_upgrade_techlab.pop_front();
            return producer_id;
        }else if(building == 2){
            auto i = starport_upgrade_techlab.begin();
            string producer_id = i->first;
            i->first = "starport_with_tech_lab_" + to_string(starport_with_tech_lab);
            starport_with_tech_lab_names_list.push_back(*i);
            starport_upgrade_techlab.pop_front();
            return producer_id;
        }
        return "";
    }

    string addOrbitalCommand(){
        orbital_command_list.push_back(orbital_command_id("orbital_command_" + to_string(orbital_command)));
        return "orbital_command_" + to_string(orbital_command);
    }

    void updateEnergy(){
        for(auto& i : orbital_command_list){
            i.updateEnergy();
        }
    }

    void muleBuild(){
        for(auto& i : orbital_command_list){
            if(i.triggerMule(this)){
                return;
            }
        }
        return;
    }

    void muleFinish(int useless){
        mule_worker -=4;
    }

    bool checkResources(int min, int supp = 0, int vesp = 0){
        if(minerals < min || vespene < vesp || (supply_max - supply_used) < supp ){
            redistributeWorkersNext(min, vesp);
            return true;
        }        
        return false;
    }
    
    void redistributeWorkersNext(double minerals_next, double vespene_next){
        double minerals_to_next = minerals_next - minerals;
        double vespene_to_next = vespene_next - vespene;
        if(vespene_to_next < 1 && minerals_to_next < 1){
            return;
        }else if(vespene_to_next < 1){
            workers_vesp = 0;
            workers_minerals = workers;
        }else if(minerals_to_next < 1){
            workers_vesp = min(workers, workers_vesp_max);
            workers_minerals = workers - workers_vesp;
        }else{
            double rate_vesp_to_all = (vespene_to_next * 2) / (vespene_to_next * 2 + minerals_to_next);
            workers_vesp = min((int) (workers * rate_vesp_to_all), workers_vesp_max);
            workers_vesp = min(workers_vesp, workers);
            workers_minerals = workers - workers_vesp;
        }
        printlist.push_front(printstruct("", ""));
        return;
    }
/*
    lists and map
*/

    list<funcBool> buildlist;
    list<finishInformation> eventlist;
    map<string, funcBool> buildmap;

    void buildBuildmap(){
        //units
        buildmap["scv"] = &Terran::scvBuild; 
        buildmap["marine"] = &Terran::marineBuild;
        buildmap["marauder"] = &Terran::marauderBuild;
        buildmap["reaper"] = &Terran::reaperBuild;
        buildmap["ghost"] = &Terran::ghostBuild;
        buildmap["hellion"] = &Terran::hellionBuild;
        buildmap["siege_tank"] = &Terran::siegeTankBuild;
        buildmap["thor"] = &Terran::thorBuild;
        buildmap["medivac"] = &Terran::medivacBuild;
        buildmap["viking"] = &Terran::vikingBuild;
        buildmap["raven"] = &Terran::ravenBuild;
        buildmap["banshee"] = &Terran::bansheeBuild;
        buildmap["battlecruiser"] = &Terran::battlecruiserBuild;
        //buildings
        buildmap["command_center"] = &Terran::commandCenterBuild;
        buildmap["orbital_command"] = &Terran::orbitalCommandBuild;
        buildmap["planetary_fortress"] = &Terran::planetaryFortressBuild;
        buildmap["refinery"] = &Terran::refineryBuild;
        buildmap["engineering_bay"] = &Terran::engineeringBayBuild;
        buildmap["missile_turret"] = &Terran::missileTurretBuild;
        buildmap["sensor_tower"] = &Terran::sensorTowerBuild;
        buildmap["barracks"] = &Terran::barracksBuild;
        buildmap["barracks_with_reactor"] = &Terran::barracksWithReactorBuild;
        buildmap["barracks_with_tech_lab"] = &Terran::barrackswithTechLabBuild;
        buildmap["factory"] = &Terran::factoryBuild;
        buildmap["factory_with_reactor"] = &Terran::factoryWithReactorBuild;
        buildmap["factory_with_tech_lab"] = &Terran::factorywithTechLabBuild;
        buildmap["armory"] = &Terran::armoryBuild;
        buildmap["bunker"] = &Terran::bunkerBuild;
        buildmap["ghost_academy"] = &Terran::ghostAcademyBuild;
        buildmap["starport"] = &Terran::starportBuild;
        buildmap["starport_with_reactor"] = &Terran::starportWithReactorBuild;
        buildmap["starport_with_tech_lab"] = &Terran::starportwithTechLabBuild;
        buildmap["fusion_core"] = &Terran::fusionCoreBuild;
        buildmap["supply_depot"] = &Terran::supplyDepotBuild;
    } 

    void addToEventlist(int time, funcInt func, int build = -1){
        finishInformation fin(time, func, build);
        eventlist.push_front(fin);
    }

    void addMuleToEventlist(int time, funcInt func, int build = -1){
        finishInformation fin(time, func, build);
        eventlist.push_back(fin);
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

    bool updateBuildlist(){
        if((this->*(*buildlist.begin()))()){
            buildlist.pop_front();
            return true;
        }
        return false;
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
        updateEnergy();
        minerals += workers_minerals * minerals_rate;
        minerals += mule_worker * minerals_rate;
        vespene += workers_vesp * vesp_rate;
    }

/*
    units
*/

    bool scvBuild(){
        if(checkResources(5000, 1) || command_center_buildslots < 1 ){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            --command_center_buildslots; 
            addToPrintlist("build-start","scv");
            addToEventlist(timestep + 17, &Terran::scvFinish);
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
        if(checkResources(5000, 1) || (barracks_buildslots < 1 && barracks_with_tech_lab_buildslots < 1 ) ){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            int slot_variable = 0;
            // checks normal barrack first, if full then into barrack with teck lab
            if(barracks_buildslots > 0){
                --barracks_buildslots;
                string producer_id = getProducerID(0, barracks_names_list, marine_id_list);
                addToPrintlist("build-start","marine", producer_id);
            }else{  
                --barracks_with_tech_lab_buildslots;
                slot_variable = 1;
                string producer_id = getProducerID(0, barracks_with_tech_lab_names_list, marine_id_list);
                addToPrintlist("build-start","marine", producer_id);
            }
            addToEventlist(timestep + 25, &Terran::marineFinish, slot_variable);
            return true; 
        }
    }

    void marineFinish(int slot){
        ++marines;
        if(slot == 0){
            ++barracks_buildslots;
            string producer_id = getProducerID(0, marine_id_list, barracks_names_list);
            addToPrintlist("build-end", "marine", "marine_" + to_string(marines), producer_id);
        }else{
            ++barracks_with_tech_lab_buildslots;
            string producer_id = getProducerID(0, marine_id_list, barracks_with_tech_lab_names_list);
            addToPrintlist("build-end", "marine", "marine_" + to_string(marines), producer_id);
        }
    }

    bool marauderBuild(){
        if(checkResources(10000, 2, 2500) || barracks_with_tech_lab_buildslots < 1){
            return false;
        }else{
            minerals -= 10000;
            vespene -= 2500;
            supply_used += 2;
            --barracks_with_tech_lab_buildslots;
            string producer_id = getProducerID(0, barracks_with_tech_lab_names_list, marauder_id_list);
            addToPrintlist("build-start", "marauder", producer_id);
            addToEventlist(timestep + 30, &Terran::marauderFinish);
            return true;
        }
    }

    void marauderFinish(int useless){
        ++marauder;
        ++barracks_with_tech_lab_buildslots;
        string producer_id = getProducerID(0, marauder_id_list, barracks_with_tech_lab_names_list);
        addToPrintlist("build-end", "marauder", "marauder_" + to_string(marauder), producer_id);
    }
    
    bool reaperBuild(){
        if(checkResources(5000, 1, 5000) || barracks_with_tech_lab_buildslots < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 5000;
            ++supply_used;
            --barracks_with_tech_lab_buildslots;
            string producer_id = getProducerID(0, barracks_with_tech_lab_names_list, reaper_id_list);
            addToPrintlist("build-start", "reaper", producer_id);
            addToEventlist(timestep + 45, &Terran::reaperFinish);
            return true;
        }
    }

    void reaperFinish(int useless){
        ++reaper;
        ++barracks_with_tech_lab_buildslots;
        string producer_id = getProducerID(0, reaper_id_list, barracks_with_tech_lab_names_list);
        addToPrintlist("build-end", "reaper", "reaper_" + to_string(reaper), producer_id);
    }

    bool ghostBuild(){
        if(checkResources(20000, 2, 10000) || barracks_with_tech_lab_buildslots < 1 || ghost_academy < 1){
            return false;
        }else{
            minerals -= 20000;
            vespene -= 10000;
            supply_used += 2;
            --barracks_with_tech_lab_buildslots;
            string producer_id = getProducerID(0, barracks_with_tech_lab_names_list, ghost_id_list);
            addToPrintlist("build-start", "ghost", producer_id);
            addToEventlist(timestep + 40, &Terran::ghostFinish);
            return true;
        }
    }

    void ghostFinish(int useless){
        ++ghost;
        ++barracks_with_tech_lab_buildslots;
        string producer_id = getProducerID(0, ghost_id_list, barracks_with_tech_lab_names_list);
        addToPrintlist("build-end", "ghost", "ghost_" + to_string(ghost), producer_id);
    }

    bool hellionBuild(){
        if(checkResources(10000, 2) || (factory_buildslots < 1 && factory_with_tech_lab_buildslots < 1)){
            return false;
        }else{
            minerals -= 10000;
            supply_used += 2;
            int slot_variable = 0;
            if(factory_buildslots > 0){
                --factory_buildslots;
                slot_variable = 1;
                string producer_id = getProducerID(1, factory_names_list, hellion_id_list);
                addToPrintlist("build-start", "hellion", producer_id);
            }else{
                --factory_with_tech_lab_buildslots;
                string producer_id = getProducerID(1, factory_with_tech_lab_names_list, hellion_id_list);
                addToPrintlist("build-start", "hellion", producer_id);
            }
            addToEventlist(timestep + 30, &Terran::hellionFinish, slot_variable);
            return true;
        }
    }

    void hellionFinish(int slot){
        ++hellion;
        if(slot == 1){
            ++factory_buildslots;
            string producer_id = getProducerID(1, hellion_id_list, factory_names_list);
            addToPrintlist("build-end", "hellion", "hellion_" + to_string(hellion), producer_id);
        }else{
            ++factory_with_tech_lab_buildslots;
            string producer_id = getProducerID(1, hellion_id_list, factory_with_tech_lab_names_list);
            addToPrintlist("build-end", "hellion", "hellion_" + to_string(hellion), producer_id);
        }
    }

    bool siegeTankBuild(){
        if(checkResources(15000, 3, 12500) || factory_with_tech_lab_buildslots < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 12500;
            supply_used += 3;
            --factory_with_tech_lab_buildslots;
            string producer_id = getProducerID(1, factory_with_tech_lab_names_list, siege_tank_id_list);
            addToPrintlist("build-start", "siege_tank", producer_id);
            addToEventlist(timestep + 45, &Terran::siegeTankFinish);
            return true;
        }
    }

    void siegeTankFinish(int useless){
        ++siege_tank;
        ++factory_with_tech_lab_buildslots;
        string producer_id = getProducerID(1, siege_tank_id_list, factory_with_tech_lab_names_list);
        addToPrintlist("build-end", "siege_tank", "siege_tank_" + to_string(siege_tank), producer_id);
    }    

    bool thorBuild(){
        if(checkResources(30000,6, 20000) || factory_with_tech_lab_buildslots < 1 || armory < 1){
            return false;
        }else{
            minerals -= 30000;
            vespene -= 20000;
            supply_used += 6;
            --factory_with_tech_lab_buildslots;
            string producer_id = getProducerID(1, factory_with_tech_lab_names_list, thor_id_list);
            addToPrintlist("build-start", "thor", producer_id);
            addToEventlist(timestep + 60, &Terran::thorFinish);
            return true;
        }
    }

    void thorFinish(int useless){
        ++thor;
        ++factory_with_tech_lab_buildslots;
        string producer_id = getProducerID(1, thor_id_list, factory_with_tech_lab_names_list);
        addToPrintlist("build-end", "thor", "thor_" + to_string(thor), producer_id);
    }    

    bool medivacBuild(){
        if(checkResources(10000, 2, 10000) || (starport_buildslots < 1 && starport_with_tech_lab_buildslots < 1)){
            return false;
        }else{
            minerals -= 10000;
            vespene -= 10000;
            supply_used += 2;
            int slot_variable = 0;
            if(starport_buildslots > 0){
                --starport_buildslots;
                string producer_id = getProducerID(2, starport_names_list, medivac_id_list);
                addToPrintlist("build-start", "medivac", producer_id);
            }else{
                --starport_with_tech_lab_buildslots;
                slot_variable = 1;
                string producer_id = getProducerID(2, starport_with_tech_lab_names_list, medivac_id_list);
                addToPrintlist("build-start", "medivac", producer_id);
            }
            addToEventlist(timestep + 42, &Terran::medivacFinish, slot_variable);
            return true;
        }
    }

    void medivacFinish(int slot){
        ++medivac;
        if(slot == 0){
            ++starport_buildslots;
            string producer_id = getProducerID(2, medivac_id_list, starport_names_list);
            addToPrintlist("build-end", "medivac", "medivac_" + to_string(medivac), producer_id);
        }else{
            ++starport_with_tech_lab_buildslots;
            string producer_id = getProducerID(2, medivac_id_list, starport_with_tech_lab_names_list);
            addToPrintlist("build-end", "medivac", "medivac_" + to_string(medivac), producer_id);
        }
        
    }   

    bool vikingBuild(){
        if(checkResources(15000, 2, 7500) || (starport_buildslots < 1 && starport_with_tech_lab_buildslots < 1)){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 7500;
            supply_used += 2;
            int slot_variable = 0;
            if(starport_buildslots > 0){
                --starport_buildslots;
                string producer_id = getProducerID(2, starport_names_list, viking_id_list);
                addToPrintlist("build-start", "viking", producer_id);

            }else{
                --starport_with_tech_lab_buildslots;
                slot_variable = 1;
                string producer_id = getProducerID(2, starport_with_tech_lab_names_list, viking_id_list);
                addToPrintlist("build-start", "viking", producer_id);

            }
            addToEventlist(timestep + 42, &Terran::vikingFinish, slot_variable);
            return true;
        }
    }

    void vikingFinish(int slot){
        ++viking;
        if(slot == 0){
            ++starport_buildslots;
            string producer_id = getProducerID(2, viking_id_list, starport_names_list);
            addToPrintlist("build-end", "viking", "viking_"+to_string(viking), producer_id);

        }else{
            ++starport_with_tech_lab_buildslots;
            string producer_id = getProducerID(2, viking_id_list, starport_with_tech_lab_names_list);
            addToPrintlist("build-end", "viking", "viking_"+to_string(viking), producer_id);
        }
    }

    bool ravenBuild(){
        if(checkResources(10000, 2, 20000) || starport_with_tech_lab_buildslots < 1){
            return false;
        }else{
            minerals -= 10000;
            vespene -= 20000;
            supply_used += 2;
            --starport_with_tech_lab_buildslots;
            string producer_id = getProducerID(2, starport_with_tech_lab_names_list, raven_id_list);
            addToPrintlist("build-start", "raven", producer_id);
            addToEventlist(timestep + 60, &Terran::ravenFinish);
            return true;
        }
    }

    void ravenFinish(int useless){
        ++raven;
        ++starport_with_tech_lab_buildslots;
        string producer_id = getProducerID(2, raven_id_list, starport_with_tech_lab_names_list);
        addToPrintlist("build-end", "raven", "raven_" + to_string(raven), producer_id);
    } 

    bool bansheeBuild(){
        if(checkResources(15000, 3, 10000) || starport_with_tech_lab_buildslots < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 10000;
            supply_used += 3;
            --starport_with_tech_lab_buildslots;
            string producer_id = getProducerID(2, starport_with_tech_lab_names_list, banshee_id_list);
            addToPrintlist("build-start", "banshee", producer_id);
            addToEventlist(timestep + 60, &Terran::bansheeFinish);
            return true;
        }
    }

    void bansheeFinish(int useless){
        ++banshee;
        ++starport_with_tech_lab_buildslots;
        string producer_id = getProducerID(2, banshee_id_list, starport_with_tech_lab_names_list);
        addToPrintlist("build-end", "banshee", "banshee_"+ to_string(banshee), producer_id);
    }

    bool battlecruiserBuild(){
        if(checkResources(40000, 6, 30000) || starport_with_tech_lab_buildslots < 1 || fusion_core < 1){
            return false;
        }else{
            minerals -= 40000;
            vespene -= 30000;
            supply_used += 6;
            --starport_with_tech_lab_buildslots;
            string producer_id = getProducerID(2, starport_with_tech_lab_names_list, battlecruiser_id_list);
            addToPrintlist("build-start", "battlecruiser", producer_id);
            addToEventlist(timestep + 90, &Terran::battlecruiserFinish);
            return true;
        }
    }

    void battlecruiserFinish(int useless){
        ++battlecruiser;
        ++starport_with_tech_lab_buildslots;
        string producer_id = getProducerID(2, battlecruiser_id_list, starport_with_tech_lab_names_list);
        addToPrintlist("build-end", "battlecruiser", "battlecruiser_" + to_string(battlecruiser), producer_id);
    } 

// ####################### end units ###################################################################################

/*
    Functions for buildings
*/

    bool commandCenterBuild(){
        if(checkResources(40000) || workers < 1){
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
        ++bases;
        ++workers;
        ++workers_minerals;
        ++command_center;
        ++command_center_buildslots;
        supply_max += 11;
        addToPrintlist("build-end", "command_center");
    }

    //TODO energie eigenschaften hinzufügen
    bool orbitalCommandBuild(){
        if(checkResources(15000) || command_center < 1 || barracks_total < 1 || command_center_buildslots < 1 ){
            return false;
        }else{
            minerals -= 15000;
            --command_center_buildslots;
            --command_center;
            addToPrintlist("build-start", "orbital_command");
            addToEventlist(timestep + 35, &Terran::orbitalCommandFinish);
            return true;
        }
    }

    void orbitalCommandFinish(int useless){
        ++orbital_command;
        ++command_center_buildslots;
        orbital_command_list.push_back("orbital_command_" + to_string(orbital_command));
        addToPrintlist("build-end", "orbital_command", "orbital_command_" + to_string(orbital_command));
    }

    bool planetaryFortressBuild(){
        if(checkResources(15000, 0, 15000) || command_center < 1 || engineering_bay < 1 || command_center_buildslots < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 15000;
            --command_center;
            --command_center_buildslots;
            addToPrintlist("build-start", "planetary_fortress");
            addToEventlist(timestep + 50, &Terran::planetaryFortressFinish);
            return true;
        }
    }

    void planetaryFortressFinish(int useless){
        ++planetary_fortress;
        ++command_center_buildslots;
        addToPrintlist("build-end", "planetary_fortress");
    }

    bool refineryBuild(){
        if(checkResources(7500) || workers < 1 || refinery + 1 > bases * geyser_max){
            return false;
        }else{
            minerals -= 7500;
            --workers;
            ++refinery;
            --workers_minerals;
            addToPrintlist("build-start","refinery");
            addToEventlist(timestep + 30, &Terran::refineryFinish);
            return true;
        }
    }

    void refineryFinish(int useless){
        ++workers_minerals;
        ++workers;
        workers_vesp_max += 3;
        addToPrintlist("build-end","refinery");
    }

    bool engineeringBayBuild(){
        if(checkResources(12500) || workers < 1){
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
        if(checkResources(10000) || workers < 1 || engineering_bay < 1){
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
        addToPrintlist("build-end", "missile_turret");
    }

    bool sensorTowerBuild(){
        if(checkResources(12500, 0, 10000) || workers < 1 || engineering_bay < 1){
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
        addToPrintlist("build-end", "sensor_tower");
    }

    bool barracksBuild(){
        if(checkResources(15000) || supply_depot < 1 || workers < 1){
            return false;
        }else{
            minerals -= 15000;
            --workers;
            --workers_minerals;
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
        addToNamesList(0, "barracks_" + to_string(barracks_total), false);
        addToPrintlist("build-end", "barracks", "barracks_" + to_string(barracks_total));
    }

    //TODO während upgrade buildslot gesperrt????
    bool barracksWithReactorBuild(){
        if(checkResources(5000, 0, 5000) || barracks < 1 || barracks_buildslots < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 5000;
            --barracks;
            --barracks_buildslots;
            string producer_id = upgradeToReactorStart(0);
            addToPrintlist("build-start", "barracks_with_reactor", producer_id);
            addToEventlist(timestep + 50, &Terran::barracksWithReactorFinish);
            return true;
        }
    }

    void barracksWithReactorFinish(int useless){
        ++barracks_with_reactor;
        barracks_buildslots += 2;
        string producer_id = upgradeToReactorFinish(0);
        addToPrintlist("build-end", "barracks_with_reactor", "barracks_with_reactor_" + to_string(barracks_with_reactor), producer_id);
    }

    bool barrackswithTechLabBuild(){
        if(checkResources(5000, 0, 2500) || barracks < 1 || barracks_buildslots < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 2500;
            --barracks;
            --barracks_buildslots;
            string producer_id = upgradeToTechLabStart(0);
            addToPrintlist("build-start", "barracks_with_tech_lab", producer_id);
            addToEventlist(timestep + 25, &Terran::barrackswithTechLabFinish);
            return true;
        }
    }

    void barrackswithTechLabFinish(int useless){
        ++barracks_with_tech_lab;
        ++barracks_with_tech_lab_buildslots;
        string producer_id = upgradeToTechLabFinish(0);
        addToPrintlist("build-end", "barracks_with_tech_lab", "barracks_with_tech_lab_" + to_string(barracks_with_tech_lab), producer_id);
    }

    bool factoryBuild(){
        if(checkResources(15000, 0, 10000) || workers < 1 || barracks_total < 1){
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
        addToNamesList(1, "factory_" + to_string(factory_total), false);
        addToPrintlist("build-end", "factory", "factory_" + to_string(factory_total));
    }

    //TODO buildslot sperren????
    bool factoryWithReactorBuild(){
        if(checkResources(5000, 0, 5000) || factory < 1 || factory_buildslots < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 5000;
            --factory;
            --factory_buildslots;
            string producer_id = upgradeToReactorStart(1);
            addToPrintlist("build-start", "factory_with_reactor", producer_id);
            addToEventlist(timestep + 50, &Terran::factoryWithReactorFinish);
            return true;
        }
    }

    void factoryWithReactorFinish(int useless){
        ++factory_with_reactor;
        factory_buildslots += 2;
        string producer_id = upgradeToReactorFinish(1);
        addToPrintlist("build-end", "factory_with_reactor", "factory_with_reactor_" + to_string(factory_with_reactor), producer_id);
    }    

    bool factorywithTechLabBuild(){
        if(checkResources(5000, 0, 2500) || factory < 1 ||factory_buildslots < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 2500;
            --factory;
            --factory_buildslots;
            string producer_id = upgradeToTechLabStart(1);
            addToPrintlist("build-start", "factory_with_tech_lab", producer_id);
            addToEventlist(timestep + 25, &Terran::factorywithTechLabFinish);
            return true;
        }
    }

    void factorywithTechLabFinish(int useless){
        ++factory_with_tech_lab;
        ++factory_with_tech_lab_buildslots;
        string producer_id = upgradeToTechLabFinish(1);
        addToPrintlist("build-end", "factory_with_tech_lab", "factory_with_tech_lab_" + to_string(factory_with_tech_lab), producer_id);
    }       

    bool armoryBuild(){
        if(checkResources(15000, 0, 10000) || workers < 1 || factory_total < 1){
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
        if(checkResources(10000) || workers < 1 || barracks_total < 1){
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
        if(checkResources(15000, 0, 5000) || workers < 1 || barracks_total < 1){
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
        addToPrintlist("build-end", "ghost_academy");
    }

    bool starportBuild(){
        if(checkResources(15000, 0, 10000) || workers < 1 || factory_total < 1){
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
        string id = "starport_" + to_string(starport_total); //
        addToNamesList(2, id, false);
        addToPrintlist("build-end", "starport", id);
    }    

    //TODO während upgrade buildslot gesperrt????
    bool starportWithReactorBuild(){
        if(checkResources(5000, 0, 5000) || starport < 1 || starport_buildslots < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 5000;
            --starport;
            --starport_buildslots;
            string producer_id = upgradeToReactorStart(2);
            addToPrintlist("build-start", "starport_with_reactor", producer_id);
            addToEventlist(timestep + 50, &Terran::starportWithReactorFinish);
            return true;
        }
    }

    void starportWithReactorFinish(int useless){
        ++starport_with_reactor;
        starport_buildslots += 2;
        string producer_id = upgradeToReactorFinish(2);
        addToPrintlist("build-end", "starport_with_reactor", "starport_with_reactor_" + to_string(starport_with_reactor), producer_id);
    }

    bool starportwithTechLabBuild(){
        if(checkResources(5000, 0, 2500) || starport < 1 || starport_buildslots < 1){
            return false;
        }else{
            minerals -= 5000;
            vespene -= 2500;
            --starport;
            --starport_buildslots;
            string producer_id = upgradeToTechLabStart(2);
            addToPrintlist("build-start", "starport_with_tech_lab", producer_id);
            addToEventlist(timestep + 25, &Terran::starportwithTechLabFinish);
            return true;
        }
    }

    void starportwithTechLabFinish(int useless){
        ++starport_with_tech_lab;
        ++starport_with_tech_lab_buildslots;
        string producer_id = upgradeToTechLabFinish(2);
        addToPrintlist("build-end", "starport_with_tech_lab", "starport_with_tech_lab_" + to_string(starport_with_tech_lab), producer_id);
    }

    bool fusionCoreBuild(){
        if(checkResources(15000, 0, 15000) || workers < 1 || starport_total < 1){
            return false;
        }else{
            minerals -= 15000;
            vespene -= 15000;
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
        if(checkResources(10000) || workers < 1){
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
    Terran(const list<string>& randomList){
        buildBuildmap();
        supply_max = 11;
        for(auto unit_name : randomList){
            if(unit_name == "") continue;
                if(buildmap.find(unit_name) == buildmap.end()){
                    cerr << "bad unitname: " << unit_name << endl;
                    exit(1);
                }
                buildlist.push_back(buildmap[unit_name]);
        }
        printHeader(1);
    }
    ~Terran() {}

    int run (){
        return testRun(1000, false);        
    }

    int testRun(int endTime, bool output = true) {
        for(;timestep < endTime;++timestep){
            updateResources();
            updateEventlist();
            bool forMule = false;
            if(!buildlist.empty()){
                forMule = updateBuildlist();
            }
            if(!forMule){
                muleBuild();
            }
            //redistributeWorkers();
            if(!printlist.empty()){
                print(timestep);
                bool eventEmpty = false;
                if(!eventlist.empty()){
                    eventEmpty = eventlist.begin()->function == &Terran::muleFinish;
                }
                if((buildlist.empty() && eventEmpty) || (buildlist.empty() && eventlist.empty())){
                    sout << endl;
                    printFinish(true);
                    return 0;
                }else{
                    sout << "," << endl;
                }
            }
        }
        printFinish(output);
        return 1;
    }

    int getEndTime(int endTime){
        // TODO: do one forward simulation and return only the endtime of the last item
        for(; timestep < endTime; ++timestep){
            updateResources();
            updateEventlist();
            bool forMule = false;
            if(!buildlist.empty()){
                forMule = updateBuildlist();
            }
            if(!forMule){
                muleBuild();
            }
            //redistributeWorkers();
            if(!printlist.empty()){
                print(timestep);
                bool eventEmpty = false;
                if(!eventlist.empty()){
                    eventEmpty = eventlist.begin()->function == &Terran::muleFinish;
                }
                if((buildlist.empty() && eventEmpty) || (buildlist.empty() && eventlist.empty())){
                    return timestep;
                }
            }
        }
        return timestep;
       
        
    }

};