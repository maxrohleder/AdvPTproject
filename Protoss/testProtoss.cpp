#include <iostream>
#include <string>
#include <list>

#include "parser_protoss.h"

using namespace std;

list<string> idlebuildings = {"nexus_0", "stargate_1", "nexus_1"};
list<string> activebuildings = {};
string type = "nexus";
int time = 42;


void print(list<string> a){
    for(string s : a){
        cout << s << " ";
    }
    cout << endl;
}

string getBuildingIdOfType(string type){
    list<string>::iterator i = find_if(idlebuildings.begin(), idlebuildings.end(), [type](const string p){return p.find(type) != std::string::npos;});
    if(i == idlebuildings.end()){
        return "error_no_empty_buildings_for_this_type" + to_string(time);
    }
    else{
        string id = *i;
        activebuildings.push_back(id);
        idlebuildings.erase(i);
        return id;
    }
}

void deactivateBuilding(string ID){
    list<string>::iterator i = find_if(activebuildings.begin(), activebuildings.end(), [ID](const string p){return p == ID;});
    if(i == activebuildings.end()){
        cout << "error";
        exit(-1);
    }
    string id = *i;
    activebuildings.erase(i);
    idlebuildings.push_back(id);
}

int main(int argc, char* argv[]){
/*     if(argc < 3  || argc > 4){
        cout << "Usage : " << argv[0] << "<techtree> <inputfile> (<debug>)" << "" << endl;
        exit(-1);
    }
    string list = string(argv[2]);
    string tree = string(argv[1]);
    bool debug = false;
    if(argc == 4) debug = (stoi(string(argv[3])) == 1);
    parser p(tree, list, debug);
    cout << validate(p, debug) << endl; */

    /* list<string> blist = {"scv", "scv", "command_center"};
    p.setBuildlist(blist); 
    cout << validate(p) << endl;*/
    print(idlebuildings);
    print(activebuildings);
    string id = getBuildingIdOfType("stargate");
    print(idlebuildings);
    print(activebuildings);
    deactivateBuilding(id);
    print(idlebuildings);
    print(activebuildings);

    return 0;
}