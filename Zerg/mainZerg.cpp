#include "../Zerg.h"
#include <iostream>
#include <string>
#include "../measureTime.h"

using namespace std;

typedef int (Zerg::*funcInt)(int x);


int main(){
    string a;
    cin >> a;
    Zerg z(a);
    int i = measureTime<Zerg, funcInt, int, int>(z, &Zerg::runTest, 1000);
    cout << i << endl;
    return 0;
}