#include <iostream>

#include "../Terran.h"

using namespace std;

int main(){
    Terran ter = Terran();
    int i = ter.testRun(5);
    cout << i <<endl;
    return 0;
}
