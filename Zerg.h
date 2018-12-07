#include "Race.h"
#include <list>
#include <map>
#include <string>
#include <algorithm>


using namespace std;
//using unitprint = pair<string, string>;


class Zerg : public Race{

    private:
    //typedef 
    typedef bool (Zerg::*funcBool) (void);
    typedef void (Zerg::*funcVoid) (void);

    int time = 1;

    list<funcBool> buildlist;
    list<pair<int, funcVoid>> eventlist;

    void droneBuild(){//missing larvae etc
        if (minerals < 5000 || (supply_max - supply_used) < 1){
            return;
        }else{
            minerals -= 5000;
            ++supply_used;
            printlist.push_front(make_pair("build-start", "drone"));
            eventlist.push_front(make_pair(time + 17, &Zerg::droneFinish));
        }
    }

    void droneFinish(){
        ++workers;
        ++workers_minerals;
        printlist.push_front(make_pair("build-end", "drone"));
    }

    //update functions
    void updateRecources(){
        minerals += workers_minerals * minerals_rate;
        vespene += workers_vesp * vesp_rate;
        //larvae update
    }

    void updateEventlist(){
        while(1){
            auto i = find_if(eventlist.begin(), eventlist.end(), [this](const pair<int, funcVoid> p){return p.first == time;});
            if(i == eventlist.end()){
                return;
            }else{
                (this->*(i->second))(); //TODO ?
                eventlist.erase(i);
            }

        };
    }

    public:

    Zerg(const string filename){
        supply_max = 200; //testpurpose
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
            droneBuild();
            updateEventlist();
            if(!printlist.empty()){
                    print(time);
            }
        }
        return 0;
    }


};