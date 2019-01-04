#include <iostream>
#include "parser.h"
#include <algorithm>

using namespace std;

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
            cout << "DEPENDENCY INVALID AT " << name << endl;
            return 1;
        }
        else if(item->produced_by != "NONE" && find(seen.begin(), seen.end(), item->produced_by) == seen.end())
        {
            cout << "PODUCED_BY INVALID AT " << name << endl;
            return 1;
        }
        else if(supply < 0 )
        {
            cout << "SUPPLY INVALID AT " << name << endl;
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

// returns 0 on successfull buildlist 1 otherwise
int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "usage: " << argv[0] << " <techtree> <buildlistfile>" << endl;
        exit(-1);
    }
    int status = validate(argv[1], argv[2]);
    if(status == 0) cout << "buildlist valid";
    else cout << "buildlist invalid";
}