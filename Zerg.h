#include "Race.h"
#include <list>
#include <map>
#include <string>


using namespace std;

class Zerg : public Race{
    private:
    

    public:
    Zerg(const string filename){}
    Zerg(const Zerg& z){}
    ~Zerg(){}
    int run(){
        for(int time = 1;time < 1000;++time){
            return 0;
        }
        return 1;
    }

};