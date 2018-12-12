#include "Race.h"

class Zerg_header : public Race{
    protected:

    double upper_relation = 1.0/2.0;
    double lower_relation = 1.0/3.0;


     //injection management

    struct queen_id{
        queen_id(string i) :energy(make_pair(250000, 2000000)), id(i){}
        queen_id(const queen_id& i) : energy(i.energy), id(i.id){}
        ~queen_id(){}
        void updateEnergy(){
            energy.first += 5625;
            if(energy.first > energy.second){
                energy.first = energy.second;
            }
        }
        pair<int, int> energy;
        string id;
    };

    list<queen_id> queen_list;

    string addQueen(){
        queen_list.push_back(queen_id("queen_" + to_string(queen)));
        return "queen_" + to_string(queen);
    }

    string getQueen(){
        for(auto& i : queen_list){
            if(i.energy.first >= 250000){
                return i.id;
            }
        }
        return "";
    }

    void useQueen(string id){
        for(auto& i : queen_list){
            if(i.id == id){
                i.energy.first -= 250000;
                return;
            }
        }
    }

    void Injection(){
        string id_queen = getQueen();
        if(id_queen == ""){
            return;
        }
        int id_hatch = injectLarvae();
        if(id_hatch < 0){
            return;
        }
        useQueen(id_queen);
        addToPrintlist("special", "injectlarvae", id_queen, "hatchery_" + to_string(id_hatch));
    }




    //larvae management
    struct larvae_pool{
        larvae_pool(int l,int i, int t = 15) : larvae_timer(t), larvae(l), id(i){}
        larvae_pool(const larvae_pool* p) : larvae_timer(p->larvae_timer), larvae(p->larvae){}
        ~larvae_pool(){}
        bool injectLarvae(){
            if((larvae + injectTimer.size()*4) > 19 || injected_larvae_producing){
                return false;
            }else{
                injected_larvae_producing = true;
                injectTimer.push_back(40);
                return true;
            }
        }
        int larvae_timer = 0;
        int larvae = 0;
        bool injected_larvae_producing = false;
        int id = 0;
        list<int> injectTimer;
    };

    list<larvae_pool> larvae_list;

    void initLarvaelist(){
        larvae_list.push_back(larvae_pool(3, bases));
    }

    void addLarvaePool(){
        larvae_list.push_back(larvae_pool(0, bases));
    }

    void updateLarvae(){
        for(auto& i :larvae_list ){
            if(i.larvae < 3){
                --(i.larvae_timer);
                if(i.larvae_timer == 0){
                    ++(i.larvae);
                    i.larvae_timer = 15;
                }
            }
            for(auto& j : i.injectTimer){
                --j;
                if(j < 1){
                    i.larvae += 4;
                }
            }
            i.injectTimer.remove(0);
        }
    }

    bool getLarvae(){
        for(auto& i : larvae_list){
            if (i.larvae > 0){
                --(i.larvae);
                return true;
            }
        }
        return false;
    }

    int injectLarvae(){
        for(auto& i : larvae_list){
            if(i.injectLarvae()){
                return i.id;
            }
        }
        return -1;
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
    int hatchery = 1;
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
        printlist.push_front(printstruct("", ""));
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
        for(auto& i : queen_list){
            i.updateEnergy();
        }
    }

    //header and finish printer

    void printHeader(int val){
        cout << "{\n\t\"buildlistValid\": " << val << "," << endl;
        cout << "\t\"game\": \"sc2-hots-zerg\"," << endl;
        cout << "\t\"messages\": [" << endl;
    }

    void printFinish(){
        cout << "\t]," << endl;
        cout << "\t\"initialUnits\": {\"drone\": [\"Fred\", \"Bob\", \"Steve\", \"Walter\", \"George\", \"Max_Musterdrone\"]," << endl;
        cout << "\t\t\t\"hatchery\": [\"hatchary_0\"]," << endl;
        cout << "\t\t\t\"overlord\": [\"Bubblehead\"]\n\t\t}" << endl;
        cout << "}" << endl;
    }

    //helpers
    //helper for printlist
    void addToPrintlist(string type, string name, string produced_id = "", string boosted_id = "") {
        if(name != "queen" && name != "hatchery" && type != "special"){
            printlist.push_back(printstruct(type, name));
        }else{
            if (type == "build-end"){
                if(name == "hatchery"){
                    printlist.push_back(printstruct(type, name, "hatchery_" + to_string(bases))); //"producedIDs" : [ "hatchery_*bases*" ]
                }else{
                    string id = addQueen();
                    printlist.push_back(printstruct(type, name, id));    //"producedIDs" : [ *id* ]
                }
            }else{
                printlist.push_back(printstruct(type, name, produced_id, boosted_id));
                //"triggeredBy" : "*produced_id*"
                //"targetBuilding : "*boosted_id*"
            }
        }
        
    }
};