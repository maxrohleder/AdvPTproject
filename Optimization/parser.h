#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <map>  
#include <list>
#include <algorithm>
#include <array>

using namespace std;

class lineObj{
    public:
        lineObj(bool vesp = false, string produced_by = "", string dependency = "") :
                 vespene(vesp), dependency(dependency), produced_by(produced_by){}
        lineObj(const lineObj& n) : vespene(n.vespene), dependency(n.dependency), produced_by(n.produced_by){}
        ~lineObj(){}

        lineObj operator=(const lineObj& n){
            vespene = n.vespene;
            dependency = n.dependency;
            produced_by = n.produced_by;
            return *this;
        }

        friend ostream& operator<<(ostream& out, const lineObj& obj);

        bool vespene; // needs vespene
        string dependency = "";
        string produced_by = "";
};

ostream& operator<<(ostream& out, const lineObj& obj){
    out << "\tvespene: " << obj.vespene << "\t\tproduced_by: " << obj.produced_by;
    if(obj.produced_by.length() < 10) out << "\t"; //for fancy output
    out << "\tdependency: " << obj.dependency;
    return out;
}

class par{
    protected:
    // ACTUAL DATASTRUCTURE
    bool debug = false;
    map<string, lineObj> dependencies;

    public:
    par(){}
    par (const string techtreefilename, bool dbg = false) :debug(dbg) {
        init(techtreefilename);
        if(debug) printMap();
    }
    par(const par& n) : debug(n.debug), dependencies(n.dependencies){}
    ~par(){}

    par operator=(const par& p){
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
            cerr << "[parser]: cant read techtree file (" << filename << ")" << endl;
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
            bool vesp = false;
            if(stoi(param[2]) != 0) vesp = true;
            dependencies[param[0]] = lineObj(vesp, param[9], param[10]);
        }
        file.close();
    }


    lineObj* get_obj(const string name){
        if(debug) cout << "getting object\n";
        if(dependencies.count(name) == 0) return NULL;
        return &dependencies[name];
    }

};
