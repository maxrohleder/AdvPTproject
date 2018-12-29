#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include <string>

bool debug = false;

struct depObj{
    depObj(std::string name, int supply_cost, int supply_provided, std::string produced_by, std::string dependency) : 
            name(name), supply(supply_provided-supply_cost), dependency(dependency), produced_by(produced_by){}
    depObj(const depObj& n) : name(n.name), supply(n.supply), dependency(n.dependency), produced_by(n.produced_by){}
    ~depObj(){}

    depObj* operator=(const depObj& n){
        name = n.name;
        supply = n.supply;
        dependency = n.dependency;
        produced_by = n.produced_by;
        return this;
    }
    
    std::string name;
    int supply; // pos if it adds negetive if it consumes
    std::string dependency;
    std::string produced_by;
};

class parser{
    protected:
    // ACTUAL DATASTRUCTURE

    public:
    std::map<std::string, depObj*> dependencies;
    std::list<std::string> buildlist;


    parser (const std::string filename,const std::string buildlistname) {
        init(filename);
        init_buildlist(buildlistname);
    }
    parser(const parser& n) : dependencies(n.dependencies){}
    ~parser(){
        //delete dependencies;
                
    }
       
    void init(const std::string filename) {
        const int MAX_LINE_LENGTH = 150;
        const char * DELIMS = ";"; // Tab, space or comma.

        std::fstream fin(filename);
        //fin.open(filename);
        if(!fin.is_open()){
            std::cerr << "cant read file";
            abort();
        } 
        // Prepare a C-string buffer to be used when reading lines.
        char buffer[MAX_LINE_LENGTH] = {};
        char *contextBuffer = NULL;        

        // Read one line at a time.
        while ( fin.getline(buffer, MAX_LINE_LENGTH) ) {
            // Extract the tokens from line:
            const char * name =             strtok_s( buffer, DELIMS, &contextBuffer);
            const char * minerals =         strtok_s( NULL, DELIMS, &contextBuffer);
            const char * vespene =          strtok_s( NULL, DELIMS, &contextBuffer);
            const char * build_time =       strtok_s( NULL, DELIMS, &contextBuffer);
            const char * supply_cost =      strtok_s( NULL, DELIMS, &contextBuffer);
            const char * supply_provided =  strtok_s( NULL, DELIMS, &contextBuffer);
            const char * start_energy =     strtok_s( NULL, DELIMS, &contextBuffer);
            const char * max_energy =       strtok_s( NULL, DELIMS, &contextBuffer);
            const char * race =             strtok_s( NULL, DELIMS, &contextBuffer);
            const char * produced_by =      strtok_s( NULL, DELIMS, &contextBuffer);
            const char * dependency =       strtok_s( NULL, DELIMS, &contextBuffer);

            // if the last tokenize operation fails 
            if(dependency == NULL){
                std::cerr << "ERROR: invalid line. MUST specify all fields. If absent, mark with either 0 or NONE\nError in line:\"" << buffer <<"\"\n";
                 continue;
            }

            // avoiding compiler warnings about unused variables
            (void) minerals;
            (void) vespene;
            (void) build_time;
            (void) start_energy;
            (void) max_energy;
            (void) race;

            // casting tokens to proper cpp types
            std::string n = std::string(name);            
            int c1 = std::atoi(supply_cost);
            int c2 = std::atoi(supply_provided); 
            std::string prod = std::string(produced_by);
            std::string dep = std::string(dependency);

            // create new element on heap
            depObj* new_Obj = new depObj(n, c1, c2, prod, dep);

            // Insert elements.
            dependencies[n] = new_Obj;

            if(debug){
                std::cout   << "{\n\t"
                            << new_Obj->name << ",\n\t" 
                            << new_Obj->supply << ",\n\t"
                            << new_Obj->produced_by << ",\n\t"
                            << new_Obj->dependency << "\n}\n";
            }

        }     
        fin.close();
    }

    depObj* get_obj(const std::string name){
        std::cout << "getting object\n";
        if(dependencies.count(name) == 0) return nullptr;
        return dependencies[name];
    }

    void init_buildlist(std::string filename){
        std::ifstream file;
        std::string item;
        file.open(filename);
        if(file.is_open()){
            while(getline(file, item)){
                if (dependencies.count(item) == 0){
                    // key does not exist. print invalid as json
                    std::cerr << "no unit named: <" << item << "> in techtree. aborting..." << std::endl;
                    exit(1);
                }
                buildlist.push_back(item);
            }
            file.close();
        }else{
            std::cerr << "could not read file" << std::endl;
            exit(-1);
        }
        file.close();
    }

    bool building(){
        return !buildlist.empty();
    }
};