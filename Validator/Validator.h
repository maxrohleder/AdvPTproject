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

    bool updateDep(){
        if(depPointerList.empty()){
            return true;
        }
        for(auto i : depPointerList){
            if(*i < 0){
                return false;
            }
        }
        ++(*newDependency);
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
    bool update(){
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

    bool updateRes(){
        if(resUpdateList.empty()){
            return true;
        }
        for(auto i : resUpdateList){
            if(!i.update()){
                return false;
            }
        }
        return true;
    }
};

//Validator for string/file input lists
class Validator{
public:
    Validator(const map<string, depObj>* depMap,const map<string, resObj>* resMap){}
    Validator(const Validator* val){}
    ~Validator(){}

    //run with readable textfile as input
    bool run(string file_name){
        //TODO
    }
    //run with already parsed buildList
    bool run(const list<string>* inputList){
        for(auto i : *inputList){
            resObj resourceObj = resMap[i];
            depObj dependencyObj = depMap[i];
            if(!dependencyObj.updateDep() || !resourceObj.updateRes()){
                return false;
            }
        }
        return true;
    }

private:
    list<string> buildList;
    //map with string as Key and struct for dependencys as Value
    //dependency object needs to have a function updateDep()
    map<string, depObj> depMap;
    //map with string as Key and struct for resource check and updating them as Value
    //resources object needs to have a function updateRes()
    map<string, resObj> resMap;

};