#include <cstring>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

struct depObj{
    depObj(std::string name, int supply_cost, int supply_provided, std::string produced_by, std::string dependency) : 
            name(name), supply(supply_provided-supply_cost), dependency(dependency), produced_by(produced_by){}
    depObj(const depObj* n) : name(n->name), supply(n->supply), dependency(n->dependency), produced_by(n->produced_by){}
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

class validator_byfile{
    protected:
    // ACTUAL DATASTRUCTURE
    std::map<std::string, depObj*> dependencies;

    public:

    validator_byfile(const std::string filename) {
        init(filename);
    }
    validator_byfile(const validator_byfile& n) : dependencies(n.dependencies){}
    ~validator_byfile(){}
     
    void print(const std::string name){
        auto item = dependencies.find(name);
        if(item != dependencies.end()){
                std::cout << "{ \t" << item->second->name << 
                ",\n\t" << item->second->supply << 
                ",\n\t" << item->second->produced_by << 
                ",\n\t" << item->second->dependency << "\n}\n";
        }
    }
       
    void init(const std::string filename) {
        const int MAX_LINE_LENGTH = 150;
        //const char * DELIMS = ";"; // Tab, space or comma.
    
        std::cout << "opening file" << std::endl;
        std::fstream fin(filename);
        //fin.open(filename);
        if(!fin.is_open()){
            std::cerr << "cant read file";
            abort();
        } 
        // Prepare a C-string buffer to be used when reading lines.
        char buffer[MAX_LINE_LENGTH] = {};
        //char *contextBuffer = NULL;        

        // Read one line at a time.
        while ( fin.getline(buffer, MAX_LINE_LENGTH) ) {
            // Extract the three tokens:
            std::cout << buffer << std::endl;
    /*         const char * name =             strtok_s( buffer, DELIMS, &contextBuffer);
            const char * minerals =         strtok_s( NULL, DELIMS, &contextBuffer);
            const char * vespene =          strtok_s( NULL, DELIMS, &contextBuffer);
            const char * build_time =       strtok_s( NULL, DELIMS, &contextBuffer);
            const char * supply_cost =      strtok_s( NULL, DELIMS, &contextBuffer);
            const char * supply_provided =  strtok_s( NULL, DELIMS, &contextBuffer);
            const char * start_energy =     strtok_s( NULL, DELIMS, &contextBuffer);
            const char * max_energy =       strtok_s( NULL, DELIMS, &contextBuffer);
            const char * race =             strtok_s( NULL, DELIMS, &contextBuffer);
            const char * produced_by =      strtok_s( NULL, DELIMS, &contextBuffer);
            const char * dependency =       strtok_s( NULL, DELIMS, &contextBuffer); */

    /*         std::string n = std::string(name);
            int c1 = std::atoi(supply_cost);
            int c2 = std::atoi(supply_provided); 
            std::string prod = std::string(produced_by);
            std::string dep = std::string(dependency); */

        }     
        // prepare test insert ( should be constructed of above tokens )
        std::string n = std::string("name");
        int c1 = std::atoi("10");
        int c2 = std::atoi("0"); 
        std::string prod = std::string("produced_by");
        std::string dep = std::string("dependency");

        depObj* new_Obj = new depObj(n, c1, c2, prod, dep);
        // Insert elements.
        dependencies["name"] = new_Obj;
        //dependencies.insert(std::pair<std::string, depObj*>(n, &new_Obj));
        // test insert ( only debugging )
        std::cout << dependencies.size() << std::endl;
        std::cout << dependencies["name"]->name <<  std::endl;
        std::cout << dependencies["name"]->supply <<  std::endl;
        std::cout << dependencies["name"]->produced_by << std::endl;
        std::cout << dependencies["name"]->dependency << std::endl;

        // Cleanup
        fin.close();
    }  

    bool set_obj(std::string name, int supply_cost, std::string prod_by, std::string dep){
        std::cout << "setting object\n";
        if(dependencies.count(name) != 0) return false;
        depObj* new_Obj = new depObj(name, supply_cost, 0, prod_by, dep);
        // Insert elements.
        dependencies[name] = new_Obj;
        return true;
    }

    depObj* get_obj(const std::string name){
        std::cout << "getting object\n";
        if(dependencies.count(name) == 0) return nullptr;
        return dependencies[name];
    }
};