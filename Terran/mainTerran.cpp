#include <iostream>

#include "../Terran.h"

using namespace std;

int main(){
    Terran ter("test");
    int i = ter.testRun(50);
    cout << i <<endl;
    return 0;
}
