#include "Race.h"

class Terran_header : public Race{

protected:

    double upper_relation = 1.0/2.0;
    double lower_relation = 1.0/3.0;
//
    void printFinish(){
        cout << "\t]\n}" << endl;
    }

    void printHeader(int val){
        cout << "{\n\t\"buildlistValid\": " << val << "," << endl;
        cout << "\t\"game\": \"sc2-hots-terran\"," << endl;
        cout << "\t\"messages\": [" << endl;
    }

    void addToPrintlist(string type, string name){
        printlist.push_back(make_pair(type, name));
    }

    void redistributeWorkers(){
        double relation = (double) workers_minerals / (double) workers;
        if(workers_vesp_max < 1 || (relation <= upper_relation && relation >= lower_relation)){
            return;
        }
        while(relation < lower_relation){
            if(workers_vesp > 0){
                --workers_vesp;
                ++workers_minerals;
                relation = (double) workers_minerals / (double) workers;
            }else{
                addToPrintlist("", "");
                return;
            }
        }
        if((workers_vesp_max - workers_vesp) < 1){
            return;
        }
        while (relation > upper_relation){
            if((workers_vesp_max - workers_vesp) < 1 ){
                break;
            }else{
                --workers_minerals;
                ++workers_vesp;
                relation = (double) workers_minerals / (double) workers;
            }
        }
        printlist.push_front(make_pair("", ""));
        return;
    }

// units
    int marines = 0;
    int marauder = 0;
    int reaper = 0;
    int ghost = 0;
    int hellion = 0;
    int siege_tank = 0;
    int thor = 0;
    int medivac = 0;
    int viking = 0;
    int raven = 0;
    int banshee = 0;
    int battlecruiser = 0;

    // buildings
    int command_center = 1;
    int orbital_command = 0;
    int planetary_fortress = 0;
    int refinery = 0;
    int engineering_bay = 0;
    int missile_turret = 0;
    int sensor_tower = 0;
    int barracks = 0;
    int factory = 0;
    int armory = 0;
    int bunker = 0;
    int ghost_academy = 0;
    int starport = 0;
    int fusion_core = 0;
    int supply_depot = 0;
    int barracks_with_reactor = 0;
    int barracks_with_tech_lab = 0;
    int starport_with_reactor = 0;
    int starport_with_tech_lab = 0;
    int factory_with_reactor = 0;
    int factory_with_tech_lab = 0;

    //counter for upgrades
    int barracks_total = 0;
    int factory_total = 0;
    int starport_total = 0;

    //buildslots
    int command_center_buildslots = 1;
    //int orbital_command_buildslots = 0; //TODO notwendig?
    //int planetary_fortress_buildslots = 0; //TODO notwendig?
    int barracks_buildslots = 0;
    int barracks_with_tech_lab_buildslots = 0;
    //int barracks_with_reactor_buildslot = 0; //TODO wirklich notwendig?
    int factory_buildslots = 0;
    int factory_with_tech_lab_buildslots = 0;
    //int factory_with_reactor_buildslot = 0; //TODO wirklich notwendig?
    int starport_buildslots = 0;
    int starport_with_tech_lab_buildslots = 0;


};