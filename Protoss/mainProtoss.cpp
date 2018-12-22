#include "../Protoss.h"
#include <iostream>
#include <string>

using namespace std;

// test with ..\..\Protoss\advancedBuild.txt as argv[1]

int main(int argc, char *argv[]){
    if(argc != 2){
        cout << "usage: " << argv[0] << "<filename>" << endl;
        exit(-1);
    }
    Protoss P(argv[1]);
    int status = P.run(1000);
    // returns sim_stat and enables differentiation
    return status;
/*     if (status == simulation_success) return simulation_success;
    if (status == simulation_invalid) return simulation_invalid;
    if (status == simulation_timeout) return simulation_timeout; */
}