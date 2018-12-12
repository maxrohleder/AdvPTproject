#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>

#include "Protoss/Protoss_header.h"

using namespace std;

class Protoss : public Protoss_header{
    private:
    //typedef 
    typedef bool (Protoss::*funcBool) (void);
    typedef void (Protoss::*funcVoid) (void);

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

    // helper functions
    void updateResources(){
        minerals += minerals_rate*workers_minerals;
        vespene += vesp_rate*workers_vesp;
    }
    bool checkResources(int min, int sup = 0, int ves = 0){
        return min<=minerals && sup<=supply_max-supply_used && ves<=vespene;
    }

    void initBuildmap(){
        buildmap["probe"] = &Protoss::probeBuild;

    }

    void buildBuildlist(const string filename){
        // first init buildmap
        initBuildmap();
        // then read in strings
        ifstream file;
        string unit_name;
        file.open(filename);
        if(file.is_open()){
            while(getline(file, unit_name)){
                if (buildmap.find(unit_name) == buildmap.end()){
                    // key does not exist. print invalid as json
                    cerr << "invalid unit in buildlist. aborting" << endl;
                    exit(1);
                }
                buildlist.push_back(buildmap[unit_name]);
            }
            file.close();
        }else{
            cerr << "could not read file" << endl;
            exit(-1);
        }
    }

    void addToPrintList(const string type, const string name){
        // TODO
    }

    void addToEventList(const int time, funcVoid func){
        // TODO
    }

    void updateEventlist(){
        while(1){
            auto i = find_if(eventlist.begin(), eventlist.end(), [this](const end_event p){return p.end_time == time;});
            if(i == eventlist.end()){
                return;
            }else{
                (this->*(i->func))();
                eventlist.erase(i);
            }

        }
    }

    void updateBuildlist(){
        if((this->*(*buildlist.begin()))()){
            buildlist.pop_front();
        }
    }

    void printHeader(int val){
        cout << "{\n\t\"buildlistValid\": " << val << "," << endl;
        cout << "\t\"game\": \"sc2-hots-zerg\"," << endl;
        cout << "\t\"messages\": [" << endl;
    }

    //build (funcbool) and finish (funcVoid) funtions
    
    bool probeBuild(){
        if(checkResources(5000, 1)){
            minerals -= 5000;
            ++supply_used;
            addToPrintList("build-start", "probe");
            addToEventList(17, &Protoss::probeFinish);
            return true;
        }
        return false;
    }

    void probeFinish(){
        addToPrintList("build-end", "probe");
    }

    public:
    Protoss(const string filename) {
        buildBuildlist(filename);
    };
    Protoss(const Protoss& p){};
    ~Protoss(){};

    int runTest(int endtime){
        
        for(; time < endtime; ++time)
        {
            updateResources();
            updateEventlist();
            // check buildlist
            if(!buildlist.empty()){
                updateBuildlist();
            }
            if(!printlist.empty()){
                print(time);
                if(buildlist.empty() && eventlist.empty()){
                    cout << "\r\t\t}  " << endl;
                    printFinish();
                    return 0;
                }else{
                    cout << endl;
                }
            }


        }
        return 0;
    }


};