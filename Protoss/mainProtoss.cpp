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
        status = P.run();
    }
    else if(argc == 4){
        Protoss P(argv[1], argv[2], atoi(argv[3]));
        status = P.run();
        if(atoi(argv[3]) == 1) cout << "status (0, 1, 2) (success/timeout/invalid):" << status << endl;
    }
    else{
        Protoss P(argv[1], "../../Validator/validator_by_file/techtree_protoss.csv");
        status = P.run();
    }
    // returns sim_stat and enables differentiation
    return 0;
/*     if (status == simulation_success) return simulation_success;
    if (status == simulation_invalid) return simulation_invalid;
    if (status == simulation_timeout) return simulation_timeout; */
}