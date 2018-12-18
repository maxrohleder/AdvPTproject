#include <iostream>
#include <string>
#include <list>
#include <map>

#include "../Race.h"

using namespace std;

// includes all stats and build functions
class Protoss_status : public Race{
    public:
    // only for debug prints
    bool debug = 0;
    int time = 1;


    protected:
    // typedefs for compatability
    typedef bool (Protoss_status::*funcBool) (void);
    typedef void (Protoss_status::*funcVoid) (void);
    //struct for eventlist
    struct end_event{
        end_event(int i, funcVoid function) : end_time(i), func(function){}
        end_event(const end_event* e) : end_time(e->end_time), func(e->func){}
        ~end_event(){}
        int end_time;
        funcVoid func;
    };
    //needed list structures
    list<funcBool> buildlist;
    list<end_event> eventlist;
    map<string, funcBool> buildmap;
    // forward declarations
    //virtual bool checkResources(int min, int sup = 0, int ves = 0){};
	//virtual void addToPrintList(string type, string name, string parent_id = "", string child_id = ""){};
    //virtual void distributeWorkers();
    //virtual void addToEventList(const int dt, funcVoid func){};
    void distributeWorkers() {
        // TODO
        ++workers_minerals;
        if(debug) cerr << "have to implement distributeWorkers" << endl;
    }

    void addToEventList (const int dt, funcVoid func) {
        eventlist.push_front(end_event(time + dt, func));
    }

    bool checkResources(int min, int sup = 0, int ves = 0) {
        return min<=minerals && sup<=supply_max-supply_used && ves<=vespene;
    }
    
    void addToPrintList (string type, string name, string parent_id = "", string child_id = "") {
        printlist.push_back(printstruct(type, name, parent_id, child_id));
    }

    //units execept for probes (in Race as worker)
    int zealot = 0;
    int stalker = 0;
    int sentry = 0;
    int warp_prism = 0;
    int immortal = 0;
    int observer = 0;
    int colossus = 0;
    int high_templar = 0;
    int dark_templar = 0;
    int mothership = 0;
    int phoenix = 0;
    int void_ray = 0;
    int carrier = 0;

    // buildings
    int nexus = 1;
    int pylon = 0;
    int gateway = 0;
    int cybernetics_core = 0;
    int robotics_facility = 0;
    int robotics_bay = 0;
    int twilight_council = 0;
    int templar_archives = 0;
    int dark_shrine = 0;
    int stargate = 0;
    int fleet_beacon = 0;
    int assimilator = 0;
    int forge = 0;
    int photon_cannon = 0;

    // probe 
    bool probeBuild(){
        if(checkResources(5000, 1)){
            minerals -= 5000;
            ++supply_used;
            addToPrintList("build-start", "probe");
            addToEventList(17, &Protoss_status::probeFinish);
            return true;
        }
        return false;
    }

    void probeFinish(){
        ++workers;
        distributeWorkers();
        addToPrintList("build-end", "probe");
    }

    // zealot
    bool zealotBuild(){
        if(checkResources(10000, 2)){
            if(gateway <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 10000;
            supply_used += 2;
            addToPrintList("build-start", "zealot");
            addToEventList(38, &Protoss_status::zealotFinish);
            return true;
        }
        return false;
    }

    void zealotFinish(){
        ++zealot;
        ++gateway;  // give free gateway place
        addToPrintList("build-end", "zealot");
    }

    // gateway
    bool gatewayBuild(){
        if(checkResources(15000)){
            if(pylon <= 0) return false;
            minerals -= 15000;
            addToEventList(65, &Protoss_status::gatewayFinish);
            addToPrintList("build-start", "gateway");
            return true;
        }
        return false;
    }

    void gatewayFinish(){
        ++gateway;
        addToPrintList("build-end", "gateway");
    }

    //nexus
    bool nexusBuild(){
        if(checkResources(40000)){
            minerals -= 40000;
            addToEventList(100, &Protoss_status::nexusFinish);
            addToPrintList("build-start", "nexus");
            return true;
        }
        return false;
    }

    void nexusFinish(){
        ++nexus;
        supply_max += 10;
        addToPrintList("build-end", "nexus");
    }

    //pylon 
    bool pylonBuild(){
        if(checkResources(10000)){
            minerals -= 10000;
            addToEventList(25, &Protoss_status::pylonFinish);
            addToPrintList("build-start", "pylon");
            return true;
        }
        return false;
    }

    void pylonFinish(){
        ++pylon;
        supply_max += 8;
        addToPrintList("build-end", "pylon");
    }
};