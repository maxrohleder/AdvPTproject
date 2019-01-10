#include <iostream>
#include <string>
#include <sstream>

#include "../Terran.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Usage : " << argv[0] << " <inputfile>" << "" << endl;
        exit(-1);
    }
    Terran ter(argv[1]);
    ter.run();
    return 0;
}
