#include "Race.h"
#include <list>
#include <map>
#include <string>


using namespace std;

class Zerg : public Race{
    private:

    void updateRecources(){
        minerals += workers_minerals * minerals_rate;
        vespene += workers_vesp * vesp_rate;
        //larvae update
    }

    public:

    Zerg(const string filename){}
    Zerg(const Zerg& z){}
    ~Zerg(){}
    int run() override{
        for(int time = 1;time < 1000;++time){
            updateRecources();
            print(time);
            return 0;
        }
        return 1;
    }

    int runTest(int endTime) {
        for(int time = 1;time < endTime;++time){
            updateRecources();
            print(time);
            return 0;
        }
        return 1;
    }


};