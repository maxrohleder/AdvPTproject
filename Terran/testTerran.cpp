#include <iostream>
#include <string>
#include <list>

#include "../Terran.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Usage : " << argv[0] << "<inputfile>" << "" << endl;
        exit(-1);
    }
    Terran ter(argv[1]);
    int i = ter.testRun(200000);
    cout << i << endl;
    return 0;
}
