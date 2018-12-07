#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>

#include "Race.h"

using namespace std;
//using unitprint = pair<string, string>;


class Terran : public Race{

public:

    Terran(string filename) {
        supply_max = 11;
    };

    ~Terran() {}


    int run (){
        for(; timestep < 1000; ++timestep){
            updateResources();
            return 0;
        }
        return 1;
    }

    int testRun (int time){
        
        for(; timestep < time; ++timestep){
            updateResources();
            scvBuild();
            updateEventlist();
            if(!printlist.empty()){
                print(timestep);
            }
        }
        return 1;
    }

private:
    int timestep = 1;

    typedef bool (Terran::*funcBool) (void);
    typedef void (Terran::*funcVoid) (void);

    list<funcBool> buildlist;
    list<pair<int, funcVoid>> eventlist;

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
                 i->second;             //TODO auto a notwendig aber nicht benutzt
                eventlist.erase(i);
            }
        }
    }

    void scvBuild(){
        //TODO Base & buildslot verfügbarkeit prüfen
        if(minerals < 5000 || ( supply_max - supply_used < 1 ) ){
            return;
        }else{
            minerals -= 5000;
            ++supply_used;
            printlist.push_front(make_pair("build-start","scv"));
            eventlist.push_front(make_pair(timestep + 17, &Terran::scvFinish));
        }
    }

    void scvFinish(){
        cout << "scv finish" <<endl;
        ++workers;
        ++workers_minerals;
        printlist.push_front(make_pair("build-finished", "scv"));
    }
    



};