#include "../Protoss.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]){
    if(argc != 2){
        cout << "usage: " << argv[0] << " + " << "<filename>" << endl;
        return 1;
    }
    string a = argv[1];
    cout << "reading in file: " << a << endl;
    Protoss P(a);
    int i = P.run(1000);
    cout << i << endl;
    return 0;
}