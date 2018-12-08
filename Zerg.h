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

    int time = 1;

    list<funcBool> buildlist;
    list<pair<int, funcVoid>> eventlist;
    map<string, funcBool> buildmap;

    
    

    bool droneBuild(){//missing larvae etc
        if (minerals < 5000 || (supply_max - supply_used) < 1){
            return false;
        }else{
            minerals -= 5000;
            ++supply_used;
            printlist.push_front(make_pair("build-start", "drone"));
            eventlist.push_front(make_pair(time + 17, &Zerg::droneFinish));
            return true;
        }
    }

    void droneFinish(){
        ++workers;
        ++workers_minerals;
        printlist.push_front(make_pair("build-end", "drone"));
    }

    bool overloredBuild(){
        if(minerals < 10000){
            return false;
        }else{
            minerals -= 10000;
            printlist.push_front(make_pair("build-start", "overlord"));
            eventlist.push_front(make_pair(time + 25, &Zerg::overlordFinish));
            return true;
        }
    }

    void overlordFinish(){
        ++overlords;
        supply_max += 8;
        printlist.push_front(make_pair("build-end", "overlord"));
    }

    //update functions
    

    void updateEventlist(){
        while(1){
            auto i = find_if(eventlist.begin(), eventlist.end(), [this](const pair<int, funcVoid> p){return p.first == time;});
            if(i == eventlist.end()){
                return;
            }else{
                (this->*(i->second))(); //TODO ?
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

    void buildTest(){
        for(int i = 0;i < 5;++i){
            buildlist.push_back(&Zerg::droneBuild);
        }
    }

    void buildBuildmap(){
        buildmap["drone"] = &Zerg::droneBuild;
        buildmap["overlord"] = &Zerg::overloredBuild;
    }

    public:

    Zerg(const string filename){
        buildBuildmap();
        supply_max = 200;
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