#include "Race.h"

class Zerg_header : public Race{
    protected:

    double upper_relation = 1.0/2.0;
    double lower_relation = 1.0/3.0;

    //larvae management
    struct larvae_pool{
        larvae_pool(int l, int t = 15) : larvae_timer(t), larvae(l){}
        larvae_pool(const larvae_pool* p) : larvae_timer(p->larvae_timer), larvae(p->larvae){}
        ~larvae_pool(){}
        int larvae_timer = 0;
        int larvae = 0;
    };

    list<larvae_pool> larvaelist;

    void initLarvaelist(){
        larvaelist.push_back(larvae_pool(3));
    }

    void addLarvaePool(){
        larvaelist.push_back(larvae_pool(0));
    }

    void updateLarvae(){
        for(auto& i :larvaelist ){
            if(i.larvae < 3){
                --(i.larvae_timer);
                if(i.larvae_timer == 0){
                    ++(i.larvae);
                    i.larvae_timer = 15;
                }
            }
        }
    }

    bool getLarvae(){
        for(auto& i : larvaelist){
            if (i.larvae > 0){
                --(i.larvae);
                return true;
            }
        }
        return false;
    }
   

    //units
    int overlord = 1;
    int overseer = 0;
    int hydralisk = 0;
    int queen = 0;
    
    int zergling = 0;
    int baneling = 0;
    int roach = 0;
    int infestor = 0;
    int nydus_worm = 0;
    int ultralisk = 0;
    int brood_lord = 0;
    int corruptor = 0;
    int mutalisk = 0;

    //buildings
    int hatchary = 1;
    int evolution_chamber = 0;
    int spore_crawler = 0;
    int lair = 0;
    int spawning_pool = 0;
    int extractor = 0;
    int spine_crawler = 0;
    int roach_warren = 0;
    int baneling_nest = 0;
    int hydralisk_den = 0;
    int infestation_pit = 0;
    int hive = 0;
    int nydus_network = 0;
    int ultralisk_cavern = 0;
    int greater_spire = 0;
    int spire = 0;

    //check for existing(morpging) structures
    int spire_greater_spire = 0;
    int lair_hive = 0;

    //build-slots for queen
    int queen_slot = 1;


    void distributeWorker(){
        ++workers_minerals;
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
                relation = workers_minerals / workers;
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
                relation = workers_minerals / workers;
            }
        }
        printlist.push_front(make_pair("", ""));
        return;
    }

    bool getWorker(){
        if(workers_minerals < 1){
            if(workers_vesp < 1){
                return false;
            }else{
                --workers_vesp;
                --workers;
                --supply_used;
            }
        }else{
            --workers_minerals;
            --workers;
            --supply_used;
        }
        return true;
    }


    void updateResources(){
        minerals += workers_minerals * minerals_rate;
        vespene += workers_vesp * vesp_rate;
        updateLarvae();
    }

    //header and finish printer

    void printHeader(int val){
        cout << "{\n\t\"buildlistValid\": " << val << "," << endl;
        cout << "\t\"game\": \"sc2-hots-zerg\"," << endl;
        cout << "\t\"messages\": [" << endl;
    }

    void printFinish(){
        cout << "\t]\n}" << endl;
    }

    //helpers
    //helper for printlist
    void addToPrintlist(string type, string name){
        printlist.push_back(make_pair(type, name));
    }
};