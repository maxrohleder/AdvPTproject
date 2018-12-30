#include "../Zerg.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

typedef int (Zerg::*funcInt)(int x);


int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Usage : " << argv[0] << " <inputfile>" << "" << endl;
        exit(-1);
    }
    Zerg z(argv[1]);
    int i = z.runTest(2000);
    return i;
}