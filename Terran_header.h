#include "Race.h"

class Terran_header : public Race{

protected:

    double upper_relation = 1.0/3.0;
    double lower_relation = 1.0/4.0;

    void printFinish(bool valid){
        if(valid){
            sout << "\t]," << endl;
            sout << "\t\"initialUnits\": {\"scv\": [\"Fred\", \"Bob\", \"Steve\", \"Walter\", \"George\", \"Muserworker\"]," << endl;
            sout << "\t\t\t\"command_center\": [\"command_center_0\"]\n\t\t}" << endl;
            sout << "}" << endl;
        }else{
            sout.str("");
            sout << "{\n\t\"game\" : \"sc2-hots-terran\",\n\t\"buildlistValid\" : \"0\"\n}" << endl;
        }
        cout << sout.str();
    }

    void printHeader(int val){
        sout << "{\n\t\"buildlistValid\": " << val << "," << endl;
        sout << "\t\"game\": \"sc2-hots-terran\"," << endl;
        sout << "\t\"messages\": [" << endl;
    }

    void addToPrintlist(string type, string name, string produced_id = "", string boosted_id = "") {
        printlist.push_back(printstruct(type, name, produced_id, boosted_id));
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
                printlist.push_front(printstruct("", ""));
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
        printlist.push_front(printstruct("", ""));
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

    int mule_worker = 0;


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
    int barracks_buildslots = 0;
    int barracks_with_tech_lab_buildslots = 0;
    int factory_buildslots = 0;
    int factory_with_tech_lab_buildslots = 0;
    int starport_buildslots = 0;
    int starport_with_tech_lab_buildslots = 0;


};