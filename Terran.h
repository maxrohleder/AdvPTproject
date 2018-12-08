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
        //TODO Base & buildslot verfügbarkeit prüfen
        if(minerals < 5000 || ( supply_max - supply_used < 1 ) ){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            printlist.push_front(make_pair("build-start","scv"));
            eventlist.push_front(make_pair(timestep + 17, &Terran::scvFinish));
            return true;
        }
    }

    void scvFinish(){
        ++workers;
        ++workers_minerals;
        printlist.push_front(make_pair("build-finished", "scv"));
    }
    
    bool marineBuild(){
        if(minerals < 50000 || ( supply_max -supply_used < 1 )){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            printlist.push_front(make_pair("build-start","marine"));
            eventlist.push_front(make_pair(timestep + 25, &Terran::marineFinish));
            return true; 
        }
    }

    void marineFinish(){
        ++marines;
        printlist.push_front(make_pair("build-finished", "marine"));
    }



public:

    Terran(const string filename){
        supply_max = 11;
        buildBuildmap();
        buildBuildlist(filename);
    }

    ~Terran() {}


    int run (){
        for(; timestep < 1000; ++timestep){
            updateResources();
            return 0;
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