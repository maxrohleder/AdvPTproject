#include "../Zerg.h"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Usage : " << argv[0] << " <inputfile>" << "" << endl;
        exit(-1);
    }
    auto start = steady_clock::now();
    Zerg z(argv[1]);
    int i = z.runTest(2000);
    auto end = steady_clock::now();
    cout << i << endl;
    cout << "time: " << duration_cast<nanoseconds>(end - start).count() << " ns" << endl;
    return 0;
}