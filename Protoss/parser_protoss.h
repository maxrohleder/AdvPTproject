#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <map>  
#include <list>
#include <algorithm>
#include <array>

using namespace std;

class dependObj{
    public:
        dependObj(string name = "", int supply = 0, bool vesp = true, string produced_by = "", string dependency = "") :
                 name(name), supply(supply), vespene(vesp), dependency(dependency), produced_by(produced_by){}
        dependObj(const dependObj& n) : name(n.name), supply(n.supply), vespene(n.vespene), dependency(n.dependency), produced_by(n.produced_by){}
        ~dependObj(){}

        dependObj& operator=(const dependObj& n){
            vespene = n.vespene;
            dependency = n.dependency;
            produced_by = n.produced_by;
            return *this;
        }

        friend ostream& operator<<(ostream& out, const dependObj& obj);


        string name;
        int supply;
        bool vespene; // needs vespene
        string dependency;
        string produced_by;
};

ostream& operator<<(ostream& out, const dependObj& obj){
    out << "\tvespene: " << obj.vespene << "\t\tproduced_by: " << obj.produced_by;
    if(obj.produced_by.length() < 10) out << "\t"; //for fancy output
    out << "\tdependency: " << obj.dependency;
    return out;
}

class parser{
    protected:
    // ACTUAL DATASTRUCTURE
    bool debug = false;

    public:
    map<string, dependObj> dependencies;
    list<string> buildlist;

    parser(){}
    parser (const string techtreefilename, bool dbg = false) :debug(dbg) {
        init(techtreefilename);
        if(debug) printMap();
    }
    parser (const string techtreefilename,const string buildlistname, bool dbg = false) : debug(dbg) {
        init(techtreefilename);
        if(debug) printMap();
        init_buildlist(buildlistname);
    }
    parser(const parser& n) : debug(n.debug), dependencies(n.dependencies), buildlist(n.buildlist){}
    ~parser(){}

    parser& operator=(const parser& p){
        debug = p.debug;
        dependencies = p.dependencies;
        return *this;
    } 

    void printMap(){
        for(auto i : dependencies){
            cout << i.first << ":";
            if(i.first.length() < 7) cout << "\t"; //for fancy output
            if(i.first.length() < 15) cout << "\t"; //for fancy output
            cout << i.second << endl;;
        }
    }

    void init(string filename){
        fstream file(filename);

        if(!file.is_open()){
            cerr << "cant read techtree file" << endl;
            exit(-1);
        } 

        string line;
        array<string, 11> param;

        while(file >> line){
            if(line == "") continue;
            size_t pos_start = 0;
            for(int i = 0; i < 11; ++i){
                size_t pos_end = line.find(';', pos_start);
                if(pos_end != string::npos){
                    param[i] = line.substr(pos_start, pos_end - pos_start);
                }else{
                    param[i] = line.substr(pos_start);
                }
                pos_start = pos_end + 1;
            }
            bool vesp = stoi(param[2]) != 0;
            int suppl = stoi(param[5])-stoi(param[4]);
            dependencies[param[0]] = dependObj(param[0], suppl, vesp, param[9], param[10]);
    //        dependObj(string name = "", int supply = 0, bool vesp = true, string produced_by = "", string dependency = "") :

        }
        file.close();
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

    dependObj* get_obj(const string name){
        if(debug) cout << "getting object\n";
        if(dependencies.count(name) == 0) return NULL;
        return &dependencies[name];
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
        dependObj& item = p.dependencies[name];
        supply += item.supply;
        // it either dependencies or produced_by are not met or supply goes below 0 return 1 and terminate
        /* if( (item.dependency != "NONE" && find(seen.begin(), seen.end(), item.dependency) == seen.end()) ||
            (item.dependency != "NONE" && find(seen.begin(), seen.end(), item.produced_by) == seen.end()) ||
            supply < 0 )
        {
            cout << "BUILDLIST INVALID AT " << name << endl;
            return 1;
        } */
        if( (item.dependency != "NONE" && find(seen.begin(), seen.end(), item.dependency) == seen.end())){
            if(debug) cout << "DEPENDENCY INVALID AT " << name << endl;
            return 1;
        }
        else if(item.produced_by != "NONE" && find(seen.begin(), seen.end(), item.produced_by) == seen.end())
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
