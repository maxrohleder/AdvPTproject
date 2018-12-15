#include "../Zerg.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

typedef int (Zerg::*funcInt)(int x);


int main(){
    string a;
    cin >> a;
    Zerg z(a);
    int i = z.runTest(1000);
    return i;
}