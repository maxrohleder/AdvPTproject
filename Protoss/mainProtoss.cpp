#include "../Protoss.h"
#include <iostream>
#include <string>

using namespace std;

// test with ..\..\Protoss\advancedBuild.txt as argv[1]

int main(int argc, char *argv[]){
    if(argc != 2 && argc != 3 && argc != 4){
        cout << "usage: " << argv[0] << " <string filename> (optional) <string techtreepath> (optional0/1) <int debug>" << endl;
        exit(-1);
    }
    int status;
    if(argc == 3){
        Protoss P(argv[1], argv[2]);
        status = P.run(1000);
    }
    else if(argc == 4){
        Protoss P(argv[1], argv[2], atoi(argv[3]));
        status = P.run(1000);
    }
    else{
        Protoss P(argv[1]);
        status = P.run(1000);
    }
    // returns sim_stat and enables differentiation
    return status;
/*     if (status == simulation_success) return simulation_success;
    if (status == simulation_invalid) return simulation_invalid;
    if (status == simulation_timeout) return simulation_timeout; */
}