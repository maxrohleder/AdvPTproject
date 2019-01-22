#pragma once
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <fstream>

using namespace std;



//struct for handling dependencys
struct depObj{
    depObj(const list<int*> depPointers = {}, int* newDependency = NULL) : depPointerList(depPointers), newDependency(newDependency){}
    depObj(const depObj* dO) : depPointerList(dO->depPointerList), newDependency(dO->newDependency){}
    ~depObj(){}

    bool updateDep(bool debug = false){
        if(debug) cout << "dep: " << endl;
        for(auto i : depPointerList){
            if (debug) cout << *i << endl;
            if(*i < 1){
                return false;
            }
        }
        if (newDependency != NULL){
            ++(*newDependency);
            if(debug) cout << "new dep: " << *newDependency << endl;
        }else{
            if (debug) cout << "no new dep" << endl;
        }
        return true;
    }

    
    private:
        list<int*> depPointerList;
        int* newDependency;

};

//pair for resource update and check
//resourceToUpdate (rTU):
struct rTU{
    int* resource;
    int toAdd;
    rTU(int* resource, int toAdd) : resource(resource), toAdd(toAdd){}
    rTU(const rTU* rU) : resource(rU->resource), toAdd(rU->toAdd){}
    ~rTU(){}
    bool update(bool debug = false){
        if(debug) cout << "resource: " << *resource << " toAdd: " << toAdd;
        *resource += toAdd;
        if (debug) cout << " result: " << *resource << endl;
        if(*resource < 0){
            return false;
        }else{
            return true;
        }
    }
};

//struct for handling resources
struct resObj{
    resObj(const list<rTU> resList = {}) : resUpdateList(resList) {}
    resObj(const resObj* rOZ) : resUpdateList(rOZ->resUpdateList){}
    ~resObj(){};
    list<rTU> resUpdateList;

    bool updateRes(bool debug = false){
        if (debug) cout << "res: " << endl;
        if (debug) cout << "resUpdateList.empty: " << resUpdateList.empty() << endl;
        if(resUpdateList.empty()){
            return true;
        }
        for(auto i : resUpdateList){
            if(!i.update(debug)){
                return false;
            }
        }
        return true;
    }
};

//Validator for string/file input lists
class Validator{
public:
    Validator(const map<string, depObj> depMap = {}, const map<string, resObj> resMap = {}) : depMap(depMap), resMap(resMap){}
    Validator(const Validator* val) : depMap(val->depMap), resMap(val->resMap){}
    ~Validator(){}

    Validator* operator=(const Validator* val){
        this->depMap = val->depMap;
        this->resMap = val->resMap;
        return this;
    }


    //run with already parsed buildList
    bool run(const list<string>* inputList, bool debug = false){
        for(auto i : *inputList){
            if(debug) cout << endl << i << ": " << endl;
            resObj resourceObj = resMap[i];
            depObj dependencyObj = depMap[i];
            bool uD = dependencyObj.updateDep(debug);
            bool uR = resourceObj.updateRes(debug);
            if (debug) cout << "updateDep: " << uD << endl;
            if (debug) cout << "updateRes: " << uR << endl;
            if(!uD || !uR){
                return false;
            }
        }
        return true;
    }

    bool runDebug(const list<string>* inputList){
        return run(inputList, 1);
    }

private:
    list<string> buildList;
    //map with string as Key and struct for dependencys as Value
    //dependency object needs to have a function updateDep()
    map<string, depObj> depMap;
    //map with string as Key and struct for resource check and updating them as Value
    //resources object needs to have a function updateRes()
    map<string, resObj> resMap;
    //just for debugging 

}; 