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
        if(depPointerList.empty()){
            return true;
        }
        if(debug) cout << "dep: " << endl;
        for(auto i : depPointerList){
            if(debug) cout << *i << endl;
            if(*i < 0){
                return false;
            }
        }
        ++(*newDependency);
        if(debug) cout << "new dep: " << *newDependency << endl;
        return true;
    }

    
    private:
        list<int*> depPointerList;
        int* newDependency;

};

//pair for resource update and check
struct resourceToUpdate{
    int* resource;
    int toAdd;
    resourceToUpdate(int* resource, int toAdd) : resource(resource), toAdd(toAdd){}
    resourceToUpdate(const resourceToUpdate* rU) : resource(rU->resource), toAdd(rU->toAdd){}
    ~resourceToUpdate(){}
    bool update(bool debug = false){
        if(debug) cout << "resource: " << *resource << " toAdd: " << toAdd << endl;
        *resource += toAdd;
        if(*resource < 0){
            return false;
        }else{
            return true;
        }
    }
};

//struct for handling resources
struct resObj{
    resObj(const list<resourceToUpdate> resList = {}) : resUpdateList(resList) {}
    resObj(const resObj* rOZ) : resUpdateList(rOZ->resUpdateList){}
    ~resObj(){};
    list<resourceToUpdate> resUpdateList;

    bool updateRes(bool debug = false){
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


    //run with already parsed buildList
    bool run(const list<string>* inputList, bool debug = false){
        for(auto i : *inputList){
            if(debug) cout << i << ": " << endl;
            resObj resourceObj = resMap[i];
            depObj dependencyObj = depMap[i];
            if(!dependencyObj.updateDep(debug) || !resourceObj.updateRes(debug)){
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