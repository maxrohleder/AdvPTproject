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

    list<funcBool> buildlist;
    list<pair<int, funcVoid>> eventlist;
    map<string, funcBool> buildmap;

    void buildBuildmap(){
        buildmap["scv"] = &Terran::scvBuild; 
        buildmap["marine"] = &Terran::marineBuild;
        buildmap["supply_Depot"] = &Terran::supplyDepotBuild;
        buildmap["barracks"] = &Terran::barracksBuild;
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
            printlist.push_front(make_pair("build-start","scv"));
            eventlist.push_front(make_pair(timestep + 17, &Terran::scvFinish));
            return true;
        }
    }

    void scvFinish(){
        ++command_center_buildslots; 
        ++workers;
        ++workers_minerals;
        printlist.push_front(make_pair("build-end", "scv"));
    }
    
    bool marineBuild(){
        if(minerals < 50000 || ( supply_max -supply_used ) < 1 || (barracks_buildslots < 1 && barracks_with_teck_lab_buildslots < 1 ) ){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            // checks normal barrack first, if full then into barrack with teck lab
            // if(barracks_buildslot > 0){
            //     --barracks_buildslot;
            // }else{  
            //     --barracks_with_teck_lab_buildslot;
            // }
            printlist.push_front(make_pair("build-start","marine"));
            eventlist.push_front(make_pair(timestep + 25, &Terran::marineFinish)); 
            return true; 
        }
    }

    void marineFinish(){
        //TODO wie weiß ich ob ich standard buildslot freigeben muss oder einen speziellen?
        ++marines;
        printlist.push_front(make_pair("build-end", "marine"));
    }

    bool barracksBuild(){
        if(minerals < 15000 || supply_depot < 1 || workers < 1){
            return false;
        }else{
            minerals -= 15000;
            --workers_minerals; //TODO Vllt im wechsel mit Vesp worker?
            printlist.push_front(make_pair("build-start", "barracks"));
            eventlist.push_front(make_pair(timestep + 65, &Terran::barracksFinish));
            return true;
        }
    }

    void barracksFinish(){
        ++barracks;
        ++barracks_buildslots; 
        ++workers_minerals;
        printlist.push_front(make_pair("build-end", "barracks"));
    }

    bool supplyDepotBuild(){
        if(minerals < 10000){
            return false;
        }else{
            minerals -= 10000;
            --workers_minerals;
            printlist.push_front(make_pair("build-start", "supply_Depot"));
            eventlist.push_front(make_pair(timestep + 30, &Terran::supplyDepotFinish));
            return true;
        }
    }

    void supplyDepotFinish(){
        ++workers_minerals;
        ++supply_depot;
        supply_max += 8;
        printlist.push_front(make_pair("build-end", "supply_Depot"));
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
        //buildlist.push_back(&Terran::scvBuild);
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