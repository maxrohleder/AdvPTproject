#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <map>
#include <math.h>
#include <climits> //added to get rid of compiler warning

#include "../Race.h"

using namespace std;

// includes all stats and build functions
class Protoss_status : public Race{
    public:
    // only for debug prints
    bool debug = true;
    int time = 1;
    // revision is state of worker distro
    bool revision_requested = true;

    void boostEnd(string ID){
        list<end_event>::iterator i = find_if(eventlist.begin(), eventlist.end(), [ID, this](const end_event p){return (p.end_time >= time && p.producerID == ID && p.boosted && p.type == "build-end");});
        if(i == eventlist.end()){
            // item was build, before chronoboost ended
            if(debug) cout << "there is no itemident produced by " << ID << " at time " << time << endl;
            return;
        }
        // normal speed results in later endtime
        if(debug)cout << "________________________CHRONO END________________________\n";
        if(debug)cout << "endtime: " << i->end_time << " time: " << time;
        i->end_time = time + (int) ceil(((double)(i->end_time-time)*1.5));
        if(debug)cout << " new endtime: " << i->end_time <<  endl;
        // reset, so that the same item could be boosted twice
        i->boosted = false;
        cout << ">>>  boosted remove <<<" << endl;
        for(auto a : boostedbuildings){
            cout << a << endl;
        }
        boostedbuildings.remove(ID);
        for(auto a : boostedbuildings){
            cout << a << endl;
        }
    }

    protected:
    // typedefs for compatability
    typedef bool (Protoss_status::*funcBool) (void);
    typedef void (Protoss_status::*funcVoid) (string ID);
    //struct for eventlist
    struct end_event{
        end_event(int i, funcVoid function, string pID = "", bool boosted = false, string type = "build-end") : end_time(i), func(function), producerID(pID), boosted(boosted), type(type){}
        end_event(const end_event* e) : end_time(e->end_time), func(e->func), producerID(e->producerID), boosted(e->boosted), type(e->type){}
        ~end_event(){}
        int end_time;
        funcVoid func;
        string producerID;
        bool boosted;
        string type;
    };
    //needed list structures
    list<funcBool> buildlist;
    list<end_event> eventlist;
    map<string, funcBool> buildmap;

    // needed for boosting building id and associated endevent
    list<string> activebuildings;
    list<string> idlebuildings;
    list<string> boostedbuildings;

    string getBuildingIdOfType(string type){
        list<string>::iterator i = find_if(idlebuildings.begin(), idlebuildings.end(), [type](const string p){return p.find(type) != std::string::npos;});
        if(i == idlebuildings.end()){
            return "error_no_empty_buildings_for_this_type" + to_string(time);
        }
        else{
            string id = *i;
            activebuildings.push_back(id);
            idlebuildings.erase(i);
            return id;
        }
    }

    void deactivateBuilding(string ID){
        list<string>::iterator i = find_if(activebuildings.begin(), activebuildings.end(), [ID](const string p){return p == ID;});
        if(i == activebuildings.end()){
            if(debug) cout << "deactivateBuilding error protoss.h";
            exit(-1);
        }
        string id = *i;
        activebuildings.erase(i);
        idlebuildings.push_back(id);
    }

    // redistribute workers, so that they are in same ratio as next planned build
    // @param report_change add blank to printlist if we are not going to build something anyway
    void distributeWorkers(int mins_needed, int vesp_needed, bool report_change) {
        // ratio over all summed resources = 0.64 = 1 - (1/(ratio+1)) = workers_min/workers
        double delta_min = max(mins_needed-minerals, 0);
        double delta_vesp = max(vesp_needed-vespene, 0);
        if(delta_min == 0 && delta_vesp == 0){
            // we already have all resources and do not need to optimize on this item
            revision_requested = false;
            return;
        }
        // VERSION 3.0 introduces division by rate which computes the needed time.
        double vesp_to_total_ratio = (double) ((double)delta_vesp/(double)vesp_rate) / (double) (((double)delta_min/(double)minerals_rate)+((double)delta_vesp/(double)vesp_rate));
        if(debug) cout << "ideal ratio(vesp/total): " << vesp_to_total_ratio << "\n\t\tdelta_min: "<<delta_min<<" delta_vesp: "<<delta_vesp;
        if(debug) cout << "\n\t\tmin: "<<mins_needed<<" vesp: "<<vesp_needed;
        // durch int casting --> fehler von ideal verteilung = max +-1
        workers_vesp = min(workers_vesp_max, (int) ((((double) workers)*vesp_to_total_ratio)+0.5));
        //if(workers_vesp < 0) cerr << "worker_distro"; exit(-1);
        workers_minerals = workers-workers_vesp;

        if(debug) cout << "\nworker distro: " << workers_minerals << " / " << workers_vesp << "(min/vesp) (vmax=" << workers_vesp_max <<")\n\n";
        if(debug && report_change) cout << "reporting\n";
        if(report_change) addToPrintList("", "");
        revision_requested = false;
    }

    void addToEventList (const int dt, funcVoid func, string producerID = "", bool boosted = false, string type = "build-end") {
        eventlist.push_front(end_event(time + dt, func, producerID, boosted, type));
    }

    // checks resources and ensures optimal worker distro
    bool checkResources(int min, int sup = 0, int ves = 0) {
         if(min<=minerals && sup<=supply_max-supply_used && ves<=vespene){
            return true;
        }
        else {
            // nur wenn nicht direkt gebaut werden konnte soll worker distributation getriggert werden 
            if(revision_requested){
                bool report = true;
                if(!printlist.empty()) report = false;
                distributeWorkers(min, ves, report);
            }
            return false;
        }

    }
    
    void addToPrintList (string type, string name, string parent_id = "", string child_id = "") {
        printlist.push_back(printstruct(type, name, parent_id, child_id));
    }

    void initBuildmap(){
        buildmap["probe"] = &Protoss_status::probeBuild;
        buildmap["zealot"] = &Protoss_status::zealotBuild;
        buildmap["stalker"] = &Protoss_status::stalkerBuild;
        buildmap["sentry"] = &Protoss_status::sentryBuild;
        buildmap["warp_prism"] = &Protoss_status::warpprismBuild;
        buildmap["immortal"] = &Protoss_status::immortalBuild;
        buildmap["observer"] = &Protoss_status::observerBuild;
        buildmap["colossus"] = &Protoss_status::colossusBuild;
        buildmap["high_templar"] = &Protoss_status::hightemplarBuild;
        buildmap["dark_templar"] = &Protoss_status::darktemplarBuild;
        buildmap["mothership"] = &Protoss_status::mothershipBuild;
        buildmap["phoenix"] = &Protoss_status::phoenixBuild;
        buildmap["void_ray"] = &Protoss_status::voidrayBuild;
        buildmap["carrier"] = &Protoss_status::carrierBuild;

        buildmap["nexus"] = &Protoss_status::nexusBuild;
        buildmap["pylon"] = &Protoss_status::pylonBuild;
        buildmap["gateway"] = &Protoss_status::gatewayBuild;
        buildmap["cybernetics_core"] = &Protoss_status::cyberneticscoreBuild;
        buildmap["robotics_facility"] = &Protoss_status::roboticsfacilityBuild;
        buildmap["robotics_bay"] = &Protoss_status::roboticsbayBuild;
        buildmap["twilight_council"] = &Protoss_status::twilightcouncilBuild;
        buildmap["templar_archives"] = &Protoss_status::templararchivesBuild;
        buildmap["dark_shrine"] = &Protoss_status::darkshrineBuild;
        buildmap["stargate"] = &Protoss_status::stargateBuild;
        buildmap["fleet_beacon"] = &Protoss_status::fleetbeaconBuild;
        buildmap["assimilator"] = &Protoss_status::assimilatorBuild;
        buildmap["forge"] = &Protoss_status::forgeBuild;
        buildmap["photon_cannon"] = &Protoss_status::photoncannonBuild;

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
            if(nexus <= 0) return false;
            --nexus;
            minerals -= 5000;
            ++supply_used;
            string producer = getBuildingIdOfType("nexus");
            addToPrintList("build-start", "probe", producer);
            addToEventList(17, &Protoss_status::probeFinish, producer);
            return true;
        }
        return false;
    }

    void probeFinish(string ID){
        ++workers;
        ++nexus;
        revision_requested = true;
        deactivateBuilding(ID);
        addToPrintList("build-end", "probe", "probe_" + to_string(workers), ID);
    }

    // zealot
    bool zealotBuild(){
        if(checkResources(10000, 2)){
            if(gateway <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 10000;
            supply_used += 2;
            string producer = getBuildingIdOfType("gateway");
            addToPrintList("build-start", "zealot",producer);
            addToEventList(38, &Protoss_status::zealotFinish, producer);
            return true;
        }
        return false;
    }

    void zealotFinish(string ID){
        ++zealot;
        ++gateway;  // give free gateway place
        deactivateBuilding(ID);
        addToPrintList("build-end", "zealot", "zealot_" + to_string(zealot), ID);
    }

    // stalker
    bool stalkerBuild(){
        if(checkResources(12500, 2, 5000)){
            if(gateway <= 0 || cybernetics_core <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 12500;
            vespene -= 5000;
            supply_used += 2;
            string producer = getBuildingIdOfType("gateway");            
            addToPrintList("build-start", "stalker", producer);
            addToEventList(42, &Protoss_status::stalkerFinish, producer);
            return true;
        }
        return false;
    }

    void stalkerFinish(string ID){
        ++stalker;
        ++gateway;  // give free gateway place
        deactivateBuilding(ID);
        addToPrintList("build-end", "stalker", "stalker_" + to_string(stalker), ID);
    }    
    //sentry
    bool sentryBuild(){
        if(checkResources(5000, 2, 10000)){
            if(gateway <= 0 || cybernetics_core <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 5000;
            vespene -= 10000;
            supply_used += 2;
            string producer = getBuildingIdOfType("gateway");
            addToPrintList("build-start", "sentry", producer);
            addToEventList(37, &Protoss_status::sentryFinish, producer);
            return true;
        }
        return false;
    }

    void sentryFinish(string ID){
        ++sentry;
        ++gateway;  // give free gateway place
        deactivateBuilding(ID);
        addToPrintList("build-end", "sentry", "sentry_" + to_string(sentry), ID);
    }      

    //warp_prism
    bool warpprismBuild(){
        if(checkResources(20000, 2)){
            if(robotics_facility <= 0) return false;
            --robotics_facility;  // occupie a place in robotics_facility
            minerals -= 20000;
            supply_used += 2;
            string producer = getBuildingIdOfType("robotics_facility");
            addToPrintList("build-start", "warp_prism", producer);
            addToEventList(50, &Protoss_status::warpprismFinish, producer);
            return true;
        }
        return false;
    }

    void warpprismFinish(string ID){
        ++warp_prism;
        ++robotics_facility;  // give free robotics_facility place
        deactivateBuilding(ID);
        addToPrintList("build-end", "warp_prism", "warp_prism_"+to_string(warp_prism), ID);
    }     
    //immortal
    bool immortalBuild(){
        if(checkResources(25000, 3, 10000)){
            if(robotics_facility <= 0) return false;
            --robotics_facility;  // occupie a place in robotics_facility
            minerals -= 25000;
            vespene -= 10000;
            supply_used += 3;
            string producer = getBuildingIdOfType("robotics_facility");
            addToPrintList("build-start", "immortal", producer);
            addToEventList(55, &Protoss_status::immortalFinish, producer);
            return true;
        }
        return false;
    }

    void immortalFinish(string ID){
        ++immortal;
        ++robotics_facility;  // give free robotics_facility place
        deactivateBuilding(ID);
        addToPrintList("build-end", "immortal", "immortal_"+to_string(immortal), ID);
    }  
    //observer
    bool observerBuild(){
        if(checkResources(2500, 1, 7500)){
            if(robotics_facility <= 0) return false;
            --robotics_facility;  // occupie a place in robotics_facility
            minerals -= 2500;
            vespene -= 7500;
            supply_used += 1;
            string producer = getBuildingIdOfType("robotics_facility");
            addToPrintList("build-start", "observer", producer);
            addToEventList(30, &Protoss_status::observerFinish, producer);
            return true;
        }
        return false;
    }

    void observerFinish(string ID){
        ++observer;
        ++robotics_facility;  // give free robotics_facility place
        deactivateBuilding(ID);
        addToPrintList("build-end", "observer", "observer_"+to_string(observer), ID);
    }  
    //colossus
    bool colossusBuild(){
        if(checkResources(30000, 6, 20000)){
            if(robotics_facility <= 0 || robotics_bay <= 0) return false;
            --robotics_facility;  // occupie a place in gateway
            minerals -= 30000;
            vespene -= 20000;
            supply_used += 6;
            string producer = getBuildingIdOfType("robotics_facility");
            addToPrintList("build-start", "colossus", producer);
            addToEventList(75, &Protoss_status::colossusFinish, producer);
            return true;
        }
        return false;
    }

    void colossusFinish(string ID){
        ++colossus;
        ++robotics_facility;  // give free gateway place
        deactivateBuilding(ID);
        addToPrintList("build-end", "colossus", "fat_boi_"+to_string(colossus), ID);
    }  
    //high_templar
    bool hightemplarBuild(){
        if(checkResources(5000, 2, 15000)){
            if(gateway <= 0 || templar_archives <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 5000;
            vespene -= 15000;
            supply_used += 2;
            string producer = getBuildingIdOfType("gateway");
            addToPrintList("build-start", "high_templar", producer);
            addToEventList(55, &Protoss_status::hightemplarFinish, producer);
            return true;
        }
        return false;
    }

    void hightemplarFinish(string ID){
        ++high_templar;
        ++gateway;  // give free gateway place
        deactivateBuilding(ID);
        addToPrintList("build-end", "high_templar", "bananenkrieger_"+to_string(high_templar), ID);
    }  
    //dark_templar
   bool darktemplarBuild(){
        if(checkResources(12500, 2, 12500)){
            if(gateway <= 0 || dark_shrine <= 0) return false;
            --gateway;  // occupie a place in gateway
            minerals -= 12500;
            vespene -= 12500;
            supply_used += 2;
            string producer = getBuildingIdOfType("gateway");
            addToPrintList("build-start", "dark_templar", producer);
            addToEventList(55, &Protoss_status::darktemplarFinish, producer);
            return true;
        }
        return false;
    }

    void darktemplarFinish(string ID){
        ++dark_templar;
        ++gateway;  // give free gateway place
        deactivateBuilding(ID);
        addToPrintList("build-end", "dark_templar", "boeser_bube_"+to_string(dark_templar), ID);
    }  
    //mothership
   bool mothershipBuild(){
        if(checkResources(40000, 8, 40000)){
            if(nexus <= 0 || fleet_beacon <= 0 || mothership != 0) return false;
            --nexus;  // occupie a place in gateway
            minerals -= 40000;
            vespene -= 40000;
            supply_used += 8;
            string producer = getBuildingIdOfType("nexus");            
            addToPrintList("build-start", "mothership", producer);
            addToEventList(160, &Protoss_status::mothershipFinish, producer);
            return true;
        }
        return false;
    }

    void mothershipFinish(string ID){
        ++mothership;
        ++nexus;  // give free gateway place
        deactivateBuilding(ID);
        addToPrintList("build-end", "mothership", "mutti_brummer_"+to_string(mothership), ID);
    }  
    //phoenix
    bool phoenixBuild(){
        if(checkResources(15000, 2, 10000)){
            if(stargate <= 0) return false;
            --stargate;  // occupie a place in stargate
            minerals -= 15000;
            vespene -= 10000;
            supply_used += 2;
            string producer = getBuildingIdOfType("stargate");
            addToPrintList("build-start", "phoenix", producer);
            addToEventList(35, &Protoss_status::phoenixFinish, producer);
            return true;
        }
        return false;
    }

    void phoenixFinish(string ID){
        ++phoenix;
        ++stargate;  // give free stargate place
        deactivateBuilding(ID);
        addToPrintList("build-end", "phoenix", "smaug_"+to_string(phoenix), ID);
    }  
    //void_ray
    bool voidrayBuild(){
        if(checkResources(25000, 3, 15000)){
            if(stargate <= 0) return false;
            --stargate;  // occupie a place in stargate
            minerals -= 25000;
            vespene -= 15000;
            supply_used += 3;
            string producer = getBuildingIdOfType("stargate");
            addToPrintList("build-start", "void_ray", producer);
            addToEventList(60, &Protoss_status::phoenixFinish, producer);
            return true;
        }
        return false;
    }

    void voidrayFinish(string ID){
        ++void_ray;
        ++stargate;  // give free stargate place
        deactivateBuilding(ID);
        addToPrintList("build-end", "void_ray", "leerer_rochen_"+to_string(void_ray), ID);
    }  
    //carrier
   bool carrierBuild(){
        if(checkResources(35000, 6, 25000)){
            if(stargate <= 0 || fleet_beacon <= 0) return false;
            --stargate;  // occupie a place in stargate
            minerals -= 35000;
            vespene -= 25000;
            supply_used += 6;
            string producer = getBuildingIdOfType("stargate");
            addToPrintList("build-start", "carrier", producer);
            addToEventList(120, &Protoss_status::carrierFinish, producer);
            return true;
        }
        return false;
    }

    void carrierFinish(string ID){
        ++carrier;
        ++stargate;  // give free stargate place
        deactivateBuilding(ID);
        addToPrintList("build-end", "carrier", "traeger_"+to_string(carrier), ID);
    }  


    // --------------------- buildings --------------------- 
    //nexus
    bool nexusBuild(){
        if(checkResources(40000)){
            minerals -= 40000;
            addToEventList(100, &Protoss_status::nexusFinish, to_string(1 + buildlist.size()));
            addToPrintList("build-start", "nexus");
            return true;
        }
        return false;
    }

    void nexusFinish(string ID){
        supply_max += 10;
        ++nexus;
        energylist.push_back(pair<int,int>(stoi(ID), 0));
        idlebuildings.push_back("nexus_" + ID);
        addToPrintList("build-end", "nexus", "nexus_" + ID);
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

    void pylonFinish(string ID){
        ++pylon;
        supply_max += 8;
        addToPrintList("build-end", "pylon");
    }
    // gateway
    bool gatewayBuild(){
        if(checkResources(15000)){
            if(pylon <= 0) return false;
            minerals -= 15000;
            addToEventList(65, &Protoss_status::gatewayFinish, to_string(1 + buildlist.size()));
            addToPrintList("build-start", "gateway");
            return true;
        }
        return false;
    }

    void gatewayFinish(string ID){
        idlebuildings.push_back("gateway_" + ID);
        addToPrintList("build-end", "gateway", "gateway_" + ID);
        ++gateway;
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

    void cyberneticscoreFinish(string ID){
        ++cybernetics_core;
        addToPrintList("build-end", "cybernetics_core");
    }

    // robotics_facility
    bool roboticsfacilityBuild(){
        if(checkResources(20000, 0, 10000)){
            if(cybernetics_core <= 0) return false;
            minerals -= 20000;
            vespene -= 10000;
            addToEventList(65, &Protoss_status::roboticsfacilityFinish, to_string(1 + buildlist.size()));
            addToPrintList("build-start", "robotics_facility");
            return true;
        }
        return false;
    }

    void roboticsfacilityFinish(string ID){
        idlebuildings.push_back("robotics_facility_" + ID);
        ++robotics_facility;
        addToPrintList("build-end", "robotics_facility", "robotics_facility_" + ID);
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

    void roboticsbayFinish(string ID){
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

    void twilightcouncilFinish(string ID){
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

    void templararchivesFinish(string ID){
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

    void darkshrineFinish(string ID){
        ++dark_shrine;
        addToPrintList("build-end", "dark_shrine");
    }

    // stargate
    bool stargateBuild(){
        if(checkResources(15000, 0, 15000)){
            if(cybernetics_core <= 0) return false;
            minerals -= 15000;
            vespene -= 15000;
            addToEventList(60, &Protoss_status::stargateFinish, to_string(1 + buildlist.size()));
            addToPrintList("build-start", "stargate");
            return true;
        }
        return false;
    }

    void stargateFinish(string ID){
        idlebuildings.push_back("stargate_" + ID);
        ++stargate;
        addToPrintList("build-end", "stargate", "stargate_" + ID);
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

    void fleetbeaconFinish(string ID){
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

    void assimilatorFinish(string ID){
        ++assimilator;
        workers_vesp_max += 3;
        revision_requested = true; // next timestep redistr. workers
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

    void forgeFinish(string ID){
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

    void photoncannonFinish(string ID){
        ++photon_cannon;
        addToPrintList("build-end", "photon_cannon");
    }
};