#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>

#include "Protoss/Protoss_status.h"

using namespace std;

// main class handling the forward of a buildlist
class Protoss : public Protoss_status{
    protected:
    //global time
    // helper functions
    void updateResources(){
        minerals += minerals_rate*workers_minerals;
        vespene += vesp_rate*workers_vesp;
    }

    void initBuildmap(){
        buildmap["probe"] = &Protoss::probeBuild;
        buildmap["zealot"] = &Protoss::zealotBuild;
        buildmap["stalker"] = &Protoss::stalkerBuild;
        buildmap["sentry"] = &Protoss::sentryBuild;
        buildmap["warp_prism"] = &Protoss::warpprismBuild;
        buildmap["immortal"] = &Protoss::immortalBuild;
        buildmap["observer"] = &Protoss::observerBuild;
        buildmap["colossus"] = &Protoss::colossusBuild;
        buildmap["high_templar"] = &Protoss::hightemplarBuild;
        buildmap["dark_templar"] = &Protoss::darktemplarBuild;
        buildmap["mothership"] = &Protoss::mothershipBuild;
        buildmap["phoenix"] = &Protoss::phoenixBuild;
        buildmap["void_ray"] = &Protoss::voidrayBuild;
        buildmap["carrier"] = &Protoss::carrierBuild;

        buildmap["nexus"] = &Protoss::nexusBuild;
        buildmap["pylon"] = &Protoss::pylonBuild;
        buildmap["gateway"] = &Protoss::gatewayBuild;
        buildmap["cybernetics_core"] = &Protoss::cyberneticscoreBuild;
        buildmap["robotics_facility"] = &Protoss::roboticsfacilityBuild;
        buildmap["robotics_bay"] = &Protoss::roboticsbayBuild;
        buildmap["twilight_council"] = &Protoss::twilightcouncilBuild;
        buildmap["templar_archives"] = &Protoss::templararchivesBuild;
        buildmap["dark_shrine"] = &Protoss::darkshrineBuild;
        buildmap["stargate"] = &Protoss::stargateBuild;
        buildmap["fleet_beacon"] = &Protoss::fleetbeaconBuild;
        buildmap["assimilator"] = &Protoss::assimilatorBuild;
        buildmap["forge"] = &Protoss::forgeBuild;
        buildmap["photon_cannon"] = &Protoss::photoncannonBuild;

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
        cout << "\t\"game\": \"sc2-hots-protoss\"," << endl;
        cout << "\t\"messages\": [" << endl;
    }

    void printFinish(){
        cout << "\t]\n}" << endl;
    }

    bool validateBuildlist(string filename){
        // TODO run through file and validate dependencies
        // return true if buildlist is valid; false if not
        bool is_valid = true;
        //check list here
        if(is_valid){
            return true;
        }else{
            cout << "{ \"game\"\t\t: \"sc2-hots-protoss\"," << endl;           
            cout << "  \"buildlistValid\"\t: \"0\"" << endl;
            cout << "}" << endl;
            return false;   
        }
    }

    public:
    Protoss(const string filename) {
        // if buildlist is invalid print json and exit(0)
        if(!validateBuildlist(filename)) exit(0);
        buildBuildlist(filename); // inits hashmap and fills it
        supply_max = 10;
    };
    Protoss(const Protoss& p){cerr << "copy constructor not support\n"; exit(1);};
    ~Protoss(){};

    int run(int endtime){
        // print header for valid game (checked in constructor)
        printHeader(1);
        for(; time < endtime; ++time)
        {
            updateResources();  //reevealuates resources
            updateEventlist();  //checks eventlist for events
            if(!buildlist.empty()) updateBuildlist();  //checks if we can build some
            if(!printlist.empty()){
                print(time);
                if(buildlist.empty() && eventlist.empty()){
                    cout << "\r\t\t}  " << endl; // get rid of ,
                    printFinish();  // end json
                    return 0;
                }else{
                    cout << endl;
                }
            }
        }
        return 0;
    }
};