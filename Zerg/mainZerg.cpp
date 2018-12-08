#include "../Zerg.h"
#include <iostream>

using namespace std;

int main(){
    Zerg z("somefile");
    int i = z.runTest(200);
    cout << i << endl;
    return 0;
}