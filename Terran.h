#include <iostream>
#include <string>
#include <list>
#include <map>

#include "Race.h"

using namespace std;

class Terran : public Race{

public:

    Terran() {}

    ~Terran() {}

    int run () override{
        for(int timestep = 1; timestep < 1000; ++timestep){
            updateResources();
            return 0;
        }
        return 1;
    }

    int testRun (int time){
        for(int timestep = 1; timestep < time; ++timestep){
            updateResources();
            cout << "1" << endl;
        }
        return 1;
    }

private:

    void updateResources(){
        minerals += workers_minerals * minerals_rate;
        vespene += workers_vesp * vesp_rate;
    }
    


    



};