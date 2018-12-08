#include "Race.h"

class Zerg_header : public Race{
    protected:
    //units
    int overlords = 0;

    //buildings


    void updateRecources(){
        minerals += workers_minerals * minerals_rate;
        vespene += workers_vesp * vesp_rate;
        //larvae update
    }
};