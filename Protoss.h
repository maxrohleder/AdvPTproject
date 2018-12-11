#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>

#include "Protoss/Protoss_header.h"

using namespace std;

class Protoss : public Protoss_header{
    private:
    //typedef 
    typedef bool (Protoss::*funcBool) (void);
    typedef void (Protoss::*funcVoid) (void);

    //struct for eventlist
    struct end_event{
        end_event(int i, funcVoid function) : end_time(i), func(function){}
        end_event(const end_event* e) : end_time(e->end_time), func(e->func){}
        ~end_event(){}
        int end_time;
        funcVoid func;
    };

    //global time
    int time = 1;

    //needed list structures
    list<funcBool> buildlist;
    list<end_event> eventlist;
    map<string, funcBool> buildmap;

    public:
    Protoss(const string build_strings){
        cout << "Protoss constructor under maintanance please come back later" << endl;
    };
    Protoss(const Protoss& p){};
    ~Protoss(){};

    int runTest(int endtime){
        
        for(; time < endtime; ++time)
        {
            /* code */
        }
        
        cout << "would simulate max of " << endtime << "seconds now" << endl;
        return 0;
    }


};