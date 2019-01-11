#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include <string>

using namespace std;

struct depObj{
    depObj(string name, int supply_cost, int supply_provided, string produced_by, string dependency) : 
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
    
    string name;
    int supply; // pos if it adds negetive if it consumes
    string dependency;
    string produced_by;
};

class parser{
    protected:
    // ACTUAL DATASTRUCTURE
    bool debug = false;

    public:
    map<string, depObj*> dependencies;
    list<string> buildlist;

    parser (const string techtreefilename,const string buildlistname, bool dbg = false) {
        debug = dbg;
        init(techtreefilename);
        init_buildlist(buildlistname);
    }
    parser(const parser& n) : debug(n.debug), dependencies(n.dependencies), buildlist(n.buildlist){}
    ~parser(){
        //delete dependencies;
        for(auto& i : dependencies)
        {
            delete i.second;
        }
    }
       
    void init(const string filename) {
        const int MAX_LINE_LENGTH = 150;
        const char * DELIMS = " ;"; // space or semicolon.

        fstream fin(filename);
        //fin.open(filename);
        if(!fin.is_open()){
            cerr << "cant read file";
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
                if(debug) cerr << "ERROR: invalid line. MUST specify all fields. If absent, mark with either 0 or NONE\nError in line: \"" << buffer <<"\"\tskipping...\n";
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
            string n = string(name);            
            int c1 = atoi(supply_cost);
            int c2 = atoi(supply_provided); 
            string prod = string(produced_by);
            string dep = string(dependency);

            // create new element on heap
            depObj* new_Obj = new depObj(n, c1, c2, prod, dep);

            // Insert elements.
            dependencies[n] = new_Obj;

            if(debug){
                cout   << "{\n\t"
                            << new_Obj->name << ",\n\t" 
                            << new_Obj->supply << ",\n\t"
                            << new_Obj->produced_by << ",\n\t"
                            << new_Obj->dependency << "\n}\n";
            }

        }     
        fin.close();
    }

    depObj* get_obj(const string name){
        if(debug) cout << "getting object\n";
        if(dependencies.count(name) == 0) return nullptr;
        return dependencies[name];
    }

    void init_buildlist(string filename){
        ifstream file;
        string item;
        file.open(filename);
        if(file.is_open()){
            while(getline(file, item)){
                if (dependencies.count(item) == 0){
                    // key does not exist. print invalid as json
                    cerr << "no unit named: <" << item << "> in techtree. aborting..." << endl;
                    exit(1);
                }
                buildlist.push_back(item);
            }
            file.close();
        }else{
            cerr << "could not read file" << endl;
            exit(-1);
        }
        file.close();
    }

    bool building(){
        return !buildlist.empty();
    }
};

// for further information on why the list is invalid set debug to true and see extensive prints
int validate(string techtree, string buildlist, bool debug = false){
    // contains all dependency objects and a list of requested build strings.
    parser p(techtree, buildlist, debug);
    // keeping track of built items
    list<string> seen = {"probe", "nexus"};
    int supply = 10;
    while(p.building()){
        string name = p.buildlist.front();
        // can do this without error checking, constructing buildlist in parser checks that they are declared in dependencies
        depObj* item = p.dependencies[name];
        supply += item->supply;
        // it either dependencies or produced_by are not met or supply goes below 0 return 1 and terminate
        /* if( (item->dependency != "NONE" && find(seen.begin(), seen.end(), item->dependency) == seen.end()) ||
            (item->dependency != "NONE" && find(seen.begin(), seen.end(), item->produced_by) == seen.end()) ||
            supply < 0 )
        {
            cout << "BUILDLIST INVALID AT " << name << endl;
            return 1;
        } */
        if( (item->dependency != "NONE" && find(seen.begin(), seen.end(), item->dependency) == seen.end())){
            if(debug) cout << "DEPENDENCY INVALID AT " << name << endl;
            return 1;
        }
        else if(item->produced_by != "NONE" && find(seen.begin(), seen.end(), item->produced_by) == seen.end())
        {
            if(debug) cout << "PODUCED_BY INVALID AT " << name << endl;
            return 1;
        }
        else if(supply < 0 )
        {
            if(debug) cout << "SUPPLY INVALID AT " << name << endl;
            return 1;
        }
        else
        {
            seen.push_front(name);
            p.buildlist.pop_front();
        }
    }
    return 0;
}