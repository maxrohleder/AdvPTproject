#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>

#include "Protoss/Protoss_status.h"

using namespace std;

enum sim_stat{
    simulation_success = 0,
    simulation_timeout = 1,
    simulation_invalid = 2
};

// main class handling the forward of a buildlist
class Protoss : public Protoss_status{
    protected:
    // helper functions
    void updateResources(){
        minerals += minerals_rate*workers_minerals;
        vespene += vesp_rate*workers_vesp;
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
                    cerr << "no unit named: <" << unit_name << "> in buildlist. aborting..." << endl;
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
            // ideal worker distro has changed with new item to build
            revision_requested = true;
        }
    }

    void printHeader(){
        sout << "{\n\t\"buildlistValid\": \"1\"," << endl;
        sout << "\t\"game\": \"sc2-hots-protoss\"," << endl;
        sout << "\t\"messages\": [" << endl;
    }

    /*does final printing from stringstream sout*/
    void printFinish(bool valid){
        if(valid){
            sout << "\t]\n}" << endl;
        }else{
            if(debug) cout << sout.str() << "\n\n\nnow real output:\n\n\n";
            sout.str("");
            sout << "{\n\t\"game\" : \"sc2-hots-protoss\",\n\t\"buildlistValid\" : \"0\"\n}" << endl;
        }
        cout << sout.str();
    }

    // checks only for hard dependencies and supply (timeout not detected)
    bool validateBuildlist(){
        auto mocked_buildlist = buildlist;
        for( funcBool item : mocked_buildlist )
        {
            mock_resources();
            if(!(this->*(item))()){
                // something could not be build despite infinite resources(excl. supply)
                if(debug) cerr << "buildlist invalid at " <<  item << "\n";
                return false;
            }                
            end_event event = eventlist.front();
            // assume instant build
            (this->*(event.func))();
            eventlist.pop_front();
            //assert(eventlist.size() == 0);
            if(debug && eventlist.size() != 0) cerr << "eventlist not empty [validation error]\n";
        }
        // successfully built all items
        default_setup();
        return true;
    }

    public:
    Protoss(const string filename) {
        buildBuildlist(filename); // inits hashmap and fills it
        supply_max = 10;
    };
    Protoss(const Protoss& p){cerr << "copy constructor not support\n"; exit(1);};
    ~Protoss(){};

    int run(int endtime = 1000){
        // print all invalid game
        if(!validateBuildlist()){
            printFinish(false);
            return simulation_invalid;   
        }
        // print header for valid game (checked in constructor)
        printHeader();
        for(; time < endtime; ++time)
        {
            updateResources();  //reevealuates resources
            updateEventlist();  //checks eventlist for events
            // TODO start stop special abilities
             //checks if we can build some and if so increment revision
            if(!buildlist.empty()) updateBuildlist(); 
            if(!printlist.empty()){
                print(time);
                if(buildlist.empty() && eventlist.empty()){
                    sout << "\r\t\t}  " << endl; // get rid of ,
                    printFinish(true);  // end json
                    return simulation_success; // simulation success
                }else{
                    sout << "," << endl;
                }
            }
        }
        // print invalid if timeout
        printFinish(false);
        return simulation_timeout; // timelimit reached; no successful simulation
    }
};