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
    depObj(const depObj& n) : name(n.name), supply(n.supply), dependency(n.dependency), produced_by(n.produced_by){}
    ~depObj(){}

    std::string name;
    int supply; // pos if it adds negetive if it consumes
    std::string dependency;
    std::string produced_by;
};

const char * DELIMS = ";"; // Tab, space or comma.
const int MAX_LINE_LENGTH = 150;

std::map< std::string, depObj*> parse_to_map(const std::string filename) {
    std::cout << "opening file" << std::endl;
    std::fstream fin(filename);
    //fin.open(filename);
    if(!fin.is_open()){
        std::cerr << "cant read file";
        abort();
    } 
    // Prepare a C-string buffer to be used when reading lines.
    char buffer[MAX_LINE_LENGTH] = {};
    char *contextBuffer = NULL;        

    // Prepare map.
    std::map< std::string, depObj*> valMap;
    std::cout << "created map" << std::endl;

    // Read one line at a time.
    while ( fin.getline(buffer, MAX_LINE_LENGTH) ) {
        // Extract the three tokens:
        std::cout << buffer << std::endl;
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

        std::string n = std::string(name);
        int c1 = std::atoi(supply_cost);
        int c2 = std::atoi(supply_provided); 
        std::string prod = std::string(produced_by);
        std::string dep = std::string(dependency);

        depObj new_Obj(n, c1, c2, prod, dep);

        // Insert elements.
        valMap[n] = &new_Obj;
    }      

    // Cleanup
    fin.close();
    return valMap;
}  
