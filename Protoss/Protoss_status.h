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
    // --------------------- units --------------------- 
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

    // stalker
    bool stalkerBuild(){
        if(checkResources(12500, 2, 5000)){
            if(gateway <= 0 || cybernetics_core <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 12500;
            vespene -= 5000;
            supply_used += 2;
            addToPrintList("build-start", "stalker");
            addToEventList(42, &Protoss_status::stalkerFinish);
            return true;
        }
        return false;
    }

    void stalkerFinish(){
        ++stalker;
        ++gateway;  // give free gateway place
        addToPrintList("build-end", "stalker");
    }    
    //sentry
    bool sentryBuild(){
        if(checkResources(5000, 2, 10000)){
            if(gateway <= 0 || cybernetics_core <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 5000;
            vespene -= 10000;
            supply_used += 2;
            addToPrintList("build-start", "sentry");
            addToEventList(37, &Protoss_status::sentryFinish);
            return true;
        }
        return false;
    }

    void sentryFinish(){
        ++sentry;
        ++gateway;  // give free gateway place
        addToPrintList("build-end", "sentry");
    }      

    //warp_prism
    bool warpprismBuild(){
        if(checkResources(20000, 2)){
            if(robotics_facility <= 0) return false;
            --robotics_facility;  // occupie a place in robotics_facility
            minerals -= 20000;
            supply_used += 2;
            addToPrintList("build-start", "warp_prism");
            addToEventList(50, &Protoss_status::warpprismFinish);
            return true;
        }
        return false;
    }

    void warpprismFinish(){
        ++warp_prism;
        ++robotics_facility;  // give free robotics_facility place
        addToPrintList("build-end", "warp_prism");
    }     
    //immortal
    bool immortalBuild(){
        if(checkResources(25000, 3, 10000)){
            if(robotics_facility <= 0) return false;
            --robotics_facility;  // occupie a place in robotics_facility
            minerals -= 25000;
            vespene -= 10000;
            supply_used += 3;
            addToPrintList("build-start", "immortal");
            addToEventList(55, &Protoss_status::immortalFinish);
            return true;
        }
        return false;
    }

    void immortalFinish(){
        ++immortal;
        ++robotics_facility;  // give free robotics_facility place
        addToPrintList("build-end", "immortal");
    }  
    //observer
    bool observerBuild(){
        if(checkResources(2500, 1, 7500)){
            if(robotics_facility <= 0) return false;
            --robotics_facility;  // occupie a place in robotics_facility
            minerals -= 2500;
            vespene -= 7500;
            supply_used += 1;
            addToPrintList("build-start", "observer");
            addToEventList(30, &Protoss_status::observerFinish);
            return true;
        }
        return false;
    }

    void observerFinish(){
        ++observer;
        ++robotics_facility;  // give free robotics_facility place
        addToPrintList("build-end", "observer");
    }  
    //colossus
    bool colossusBuild(){
        if(checkResources(30000, 6, 20000)){
            if(robotics_facility <= 0 || robotics_bay <= 0) return false;
            --robotics_facility;  // occupie a place in gateway
            minerals -= 30000;
            vespene -= 20000;
            supply_used += 6;
            addToPrintList("build-start", "colossus");
            addToEventList(75, &Protoss_status::colossusFinish);
            return true;
        }
        return false;
    }

    void colossusFinish(){
        ++colossus;
        ++robotics_facility;  // give free gateway place
        addToPrintList("build-end", "colossus");
    }  
    //high_templar
    bool hightemplarBuild(){
        if(checkResources(5000, 2, 15000)){
            if(gateway <= 0 || templar_archives <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 5000;
            vespene -= 15000;
            supply_used += 2;
            addToPrintList("build-start", "high_templar");
            addToEventList(55, &Protoss_status::hightemplarFinish);
            return true;
        }
        return false;
    }

    void hightemplarFinish(){
        ++high_templar;
        ++gateway;  // give free gateway place
        addToPrintList("build-end", "high_templar");
    }  
    //dark_templar
   bool darktemplarBuild(){
        if(checkResources(12500, 2, 12500)){
            if(gateway <= 0 || dark_shrine <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 12500;
            vespene -= 12500;
            supply_used += 2;
            addToPrintList("build-start", "dark_templar");
            addToEventList(55, &Protoss_status::darktemplarFinish);
            return true;
        }
        return false;
    }

    void darktemplarFinish(){
        ++dark_templar;
        ++gateway;  // give free gateway place
        addToPrintList("build-end", "dark_templar");
    }  
    //mothership
   bool mothershipBuild(){
        if(checkResources(40000, 8, 40000)){
            if(nexus <= 0 || fleet_beacon <= 0) return false;
            --nexus;  // occupie a place in gateway
            minerals -= 40000;
            vespene -= 40000;
            supply_used += 8;
            addToPrintList("build-start", "mothership");
            addToEventList(160, &Protoss_status::mothershipFinish);
            return true;
        }
        return false;
    }

    void mothershipFinish(){
        ++mothership;
        ++nexus;  // give free gateway place
        addToPrintList("build-end", "mothership");
    }  
    //phoenix
    bool phoenixBuild(){
        if(checkResources(15000, 2, 10000)){
            if(stargate <= 0) return false;
            --stargate;  // occupie a place in stargate
            minerals -= 15000;
            vespene -= 10000;
            supply_used += 2;
            addToPrintList("build-start", "phoenix");
            addToEventList(35, &Protoss_status::phoenixFinish);
            return true;
        }
        return false;
    }

    void phoenixFinish(){
        ++phoenix;
        ++stargate;  // give free stargate place
        addToPrintList("build-end", "phoenix");
    }  
    //void_ray
    bool voidrayBuild(){
        if(checkResources(25000, 3, 15000)){
            if(stargate <= 0) return false;
            --stargate;  // occupie a place in stargate
            minerals -= 25000;
            vespene -= 15000;
            supply_used += 3;
            addToPrintList("build-start", "void_ray");
            addToEventList(60, &Protoss_status::phoenixFinish);
            return true;
        }
        return false;
    }

    void voidrayFinish(){
        ++void_ray;
        ++stargate;  // give free stargate place
        addToPrintList("build-end", "void_ray");
    }  
    //carrier
   bool carrierBuild(){
        if(checkResources(35000, 6, 25000)){
            if(stargate <= 0 || fleet_beacon <= 0) return false;
            --stargate;  // occupie a place in stargate
            minerals -= 35000;
            vespene -= 25000;
            supply_used += 6;
            addToPrintList("build-start", "carrier");
            addToEventList(120, &Protoss_status::carrierFinish);
            return true;
        }
        return false;
    }

    void carrierFinish(){
        ++carrier;
        ++stargate;  // give free stargate place
        addToPrintList("build-end", "carrier");
    }  


    // --------------------- buildings --------------------- 
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

    // cybernetics core
    bool cyberneticscoreBuild(){
        if(checkResources(15000)){
            if(gateway <= 0) return false;
            minerals -= 15000;
            addToEventList(50, &Protoss_status::cyberneticscoreFinish);
            addToPrintList("build-start", "cybernetics_core");
            return true;
        }
        return false;
    }

    void cyberneticscoreFinish(){
        ++cybernetics_core;
        addToPrintList("build-end", "cybernetics_core");
    }

    // robotics_facility
    bool roboticsfacilityBuild(){
        if(checkResources(20000, 0, 10000)){
            if(cybernetics_core <= 0) return false;
            minerals -= 20000;
            vespene -= 10000;
            addToEventList(65, &Protoss_status::roboticsfacilityFinish);
            addToPrintList("build-start", "robotics_facility");
            return true;
        }
        return false;
    }

    void roboticsfacilityFinish(){
        ++robotics_facility;
        addToPrintList("build-end", "robotics_facility");
    }

    // robotics_bay
    bool roboticsbayBuild(){
        if(checkResources(20000, 0, 20000)){
            if(robotics_facility <= 0) return false;
            minerals -= 20000;
            vespene -= 20000;
            addToEventList(65, &Protoss_status::roboticsbayFinish);
            addToPrintList("build-start", "robotics_bay");
            return true;
        }
        return false;
    }

    void roboticsbayFinish(){
        ++robotics_bay;
        addToPrintList("build-end", "robotics_bay");
    }

    // twilight_council
    bool twilightcouncilBuild(){
        if(checkResources(15000, 0, 10000)){
            if(cybernetics_core <= 0) return false;
            minerals -= 15000;
            vespene -= 10000;
            addToEventList(50, &Protoss_status::twilightcouncilFinish);
            addToPrintList("build-start", "twilight_council");
            return true;
        }
        return false;
    }

    void twilightcouncilFinish(){
        ++twilight_council;
        addToPrintList("build-end", "twilight_council");
    }

    // templar_archives
    bool templararchivesBuild(){
        if(checkResources(15000, 0, 20000)){
            if(twilight_council <= 0) return false;
            minerals -= 15000;
            vespene -= 20000;
            addToEventList(50, &Protoss_status::templararchivesFinish);
            addToPrintList("build-start", "templar_archives");
            return true;
        }
        return false;
    }

    void templararchivesFinish(){
        ++templar_archives;
        addToPrintList("build-end", "templar_archives");
    }

    // dark_shrine
    bool darkshrineBuild(){
        if(checkResources(10000, 0, 15000)){
            if(twilight_council <= 0) return false;
            minerals -= 10000;
            vespene -= 15000;
            addToEventList(100, &Protoss_status::darkshrineFinish);
            addToPrintList("build-start", "dark_shrine");
            return true;
        }
        return false;
    }

    void darkshrineFinish(){
        ++dark_shrine;
        addToPrintList("build-end", "dark_shrine");
    }

    // stargate
    bool stargateBuild(){
        if(checkResources(15000, 0, 15000)){
            if(cybernetics_core <= 0) return false;
            minerals -= 15000;
            vespene -= 15000;
            addToEventList(60, &Protoss_status::stargateFinish);
            addToPrintList("build-start", "stargate");
            return true;
        }
        return false;
    }

    void stargateFinish(){
        ++stargate;
        addToPrintList("build-end", "stargate");
    }

    // fleet_beacon
    bool fleetbeaconBuild(){
        if(checkResources(30000, 0, 20000)){
            if(stargate <= 0) return false;
            minerals -= 30000;
            vespene -= 20000;
            addToEventList(60, &Protoss_status::fleetbeaconFinish);
            addToPrintList("build-start", "fleet_beacon");
            return true;
        }
        return false;
    }

    void fleetbeaconFinish(){
        ++fleet_beacon;
        addToPrintList("build-end", "fleet_beacon");
    }
       
    // assimilator
    bool assimilatorBuild(){
        if(checkResources(7500)){
            minerals -= 7500;
            addToEventList(30, &Protoss_status::assimilatorFinish);
            addToPrintList("build-start", "assimilator");
            return true;
        }
        return false;
    }

    void assimilatorFinish(){
        ++assimilator;
        addToPrintList("build-end", "assimilator");
    }

    // forge
    bool forgeBuild(){
        if(checkResources(15000)){
            if(pylon <= 0) return false;
            minerals -= 15000;
            addToEventList(45, &Protoss_status::forgeFinish);
            addToPrintList("build-start", "forge");
            return true;
        }
        return false;
    }

    void forgeFinish(){
        ++forge;
        addToPrintList("build-end", "forge");
    }
    
    // photon_cannon
    bool photoncannonBuild(){
        if(checkResources(15000)){
            if(forge <= 0) return false;
            minerals -= 15000;
            addToEventList(40, &Protoss_status::photoncannonFinish);
            addToPrintList("build-start", "photon_cannon");
            return true;
        }
        return false;
    }

    void photoncannonFinish(){
        ++photon_cannon;
        addToPrintList("build-end", "photon_cannon");
    }
};