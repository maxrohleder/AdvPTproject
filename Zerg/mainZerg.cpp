#include "../Zerg.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Usage : " << argv[0] << " <inputfile>" << "" << endl;
        exit(-1);
    }
    Zerg z(argv[1]);
    z.run();
    return 0;
}