#include "../Zerg.h"
#include <iostream>

using namespace std;

int main(){
    Zerg z("somefile");
    int i = z.runTest(50);
    cout << i << endl;
    cout << "This is Sparta!" << endl;
    return 0;
}