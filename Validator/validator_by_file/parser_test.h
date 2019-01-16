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
        lineObj(int supply_cost = 0, int supply_provided = 0, string produced_by = "", string dependency = "") :
                 supply(supply_provided-supply_cost), dependency(dependency), produced_by(produced_by){}
        lineObj(const lineObj& n) : supply(n.supply), dependency(n.dependency), produced_by(n.produced_by){}
        ~lineObj(){}

        lineObj& operator=(const lineObj& n){
            supply = n.supply;
            dependency = n.dependency;
            produced_by = n.produced_by;
            return *this;
        }

        friend ostream& operator<<(ostream& out, const lineObj& obj);

    protected:
        int supply = 0; // pos if it adds negetive if it consumes
        string dependency = "";
        string produced_by = "";
};

ostream& operator<<(ostream& out, const lineObj& obj){
    out << "\tsupply: " << obj.supply << "\t\tproduced_by: " << obj.produced_by;
    if(obj.produced_by.length() < 10) out << "\t"; //for fancy output
    out << "\tdependency: " << obj.dependency;
    return out;
}

class parser{
    protected:
    // ACTUAL DATASTRUCTURE
    bool debug = false;
    map<string, lineObj> dependencies;

    public:
    parser (const string techtreefilename = "", bool dbg = false) :debug(dbg) {
        init(techtreefilename);
        if(debug) printMap();
    }
    parser(const parser& n) : debug(n.debug), dependencies(n.dependencies){}
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

    void init(const string filename){
        fstream file(filename);

        if(!file.is_open()){
            cerr << "cant read techtree file";
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
            dependencies[param[0]] = lineObj(stoi(param[4]), stoi(param[5]), param[9], param[10]);
        }
        file.close();
    }


    lineObj* get_obj(const string name){
        if(debug) cout << "getting object\n";
        if(dependencies.count(name) == 0) return NULL;
        return &dependencies[name];
    }

};
