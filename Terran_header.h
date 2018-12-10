#include "Race.h"

class Terran_header : public Race{

protected:
//
    void printFinish(){
        cout << "\t]\n}" << endl;
    }

    void printHeader(int val){
        cout << "{\n\t\"buildlistValid\": " << val << "," << endl;
        cout << "\t\"game\": \"sc2-hots-terran\"," << endl;
        cout << "\t\"messages\": [" << endl;
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
    int barracks_with_teck_lab = 0;
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
    int barracks_with_teck_lab_buildslots = 0;
    //int barracks_with_reactor_buildslot = 0; //TODO wirklich notwendig?
    int factory_buildslots = 0;
    int factory_with_teck_lab_buildslots = 0;
    //int factory_with_reactor_buildslot = 0; //TODO wirklich notwendig?
    int starport_buildslots = 0;
    int starport_with_teck_lab_buildslots = 0;


};