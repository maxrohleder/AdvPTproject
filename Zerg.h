#include "Zerg_header.h"
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>


using namespace std;


class Zerg : public Zerg_header{

    private:
    //typedef 
    typedef bool (Zerg::*funcBool) (void);
    typedef void (Zerg::*funcVoid) (void);

    //struct for eventlist
    struct end_event{
        end_event(int i, funcVoid function) : end_time(i), func(function){}
        end_event(const end_event* e) : end_time(e->end_time), func(e->func){}
        ~end_event(){}
        int end_time;
        funcVoid func;
    };

    //global time
    int time = 1;

    //needed list structures
    list<funcBool> buildlist;
    list<end_event> eventlist;
    map<string, funcBool> buildmap;


    //helpers
    //helper for printlist
    void addToPrintlist(string type, string name){
        printlist.push_back(make_pair(type, name));
    }

    //helper for eventlist
    void addToEventlist(int t, funcVoid func){
        eventlist.push_front(end_event(time + t, func));
    }

    //helper for resource-check
    bool checkResources(int min, int supp, int vesp = 0){
        if(minerals < min || vespene < vesp || (supply_max - supply_used) < supp ){
            return false;
        }
        return true;
    }
    
    
    //build and finish functions

    //drone

    bool droneBuild(){
        if (checkResources(5000, 1)){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 5000;
            ++supply_used;
            addToPrintlist("build-start", "drone");
            addToEventlist(17, &Zerg::droneFinish);
            return true;
        }
    }

    void droneFinish(){
        ++workers;
        distributeWorker();
        addToPrintlist("build-end", "drone");
    }

    //overlord

    bool overloredBuild(){
        if(checkResources(10000, 0)){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 10000;
            addToPrintlist("build-start", "overlord");
            addToEventlist(25, &Zerg::overlordFinish);
            return true;
        }
    }

    void overlordFinish(){
        ++overlord;
        supply_max += 8;
        addToPrintlist("build-end", "overlord");
    }

    //overseer

    bool overseerBuild(){
        if (checkResources(5000, 0, 5000) || overlord < 1 || (lair < 1 && hive < 0)){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 5000;
            vespene -= 5000;
            --overlord;
            addToPrintlist("build-start", "overseer");
            addToEventlist(17, &Zerg::overseerFinish);
            return true;
        }
    }

    void overseerFinish(){
        ++overseer;
        addToPrintlist("build-end", "overseer");
    }

    //hydralisk

    bool hydraliskBuild() {
        if(checkResources(10000, 2, 5000) || hydralisk_den < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 10000;
            vespene -= 5000;
            supply_used += 2;
            addToPrintlist("build-start", "hydralisk");
            addToEventlist(33, &Zerg::hydraliskFinish);
            return true;
        }
    }

    void hydraliskFinish(){
        ++hydralisk;
        addToPrintlist("build-end", "hydralisk");
    }

    //zergling

    bool zerglingBuild(){
        if(checkResources(5000, 5) || spawning_pool < 1){
            return false;
        }else{
            if(!getLarvae()){
                return false;
            }
            minerals -= 5000;
            supply_used += 10;
            addToPrintlist("build-start", "zergling");
            addToEventlist(24, &Zerg::zerglingFinish);
            return true;
        }
    }

    void zerglingFinish(){
        zergling += 2;
        addToPrintlist("build-end", "zergling");
    }

    //baneling

    bool banelingBuild(){
        if(checkResources(2500, 0, 2500) || zergling < 1 || baneling_nest < 1){
            return false;
        }else{
            minerals -= 2500;
            vespene -= 2500;
            --zergling;
            addToPrintlist("build-start", "baneling");
            addToEventlist(20, &Zerg::banelingFinish);
            return true;
        }
    }

    void banelingFinish(){
        ++baneling;
        addToPrintlist("build-end", "baneling");
    }



    //update functions
    

    void updateEventlist(){
        while(1){
            auto i = find_if(eventlist.begin(), eventlist.end(), [this](const pair<int, funcVoid> p){return p.first == time;});
            if(i == eventlist.end()){
                return;
            }else{
                (this->*(i->func))(); //TODO ?
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

    void buildBuildmap(){
        buildmap["drone"] = &Zerg::droneBuild;
        buildmap["overlord"] = &Zerg::overloredBuild;
        buildmap["overseer"] = &Zerg::overseerBuild;
        buildmap["hydralisk"] = &Zerg::hydraliskBuild;
        buildmap["zergling"] = &Zerg::zerglingBuild;
        buildmap["baneling"] = &Zerg::banelingBuild;
    }

    public:

    Zerg(const string filename){
        buildBuildmap();
        initLarvaelist();
        supply_max = 10;
        buildBuildlist(filename);
        //test purpose
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
            updateEventlist();
            if(!buildlist.empty()){
                updateBuildlist();
            }
            
            if(!printlist.empty()){
                    print(time);
            }
            if(buildlist.empty() && eventlist.empty()){
                return 0;
            }
        }
        return 1;
    }


};