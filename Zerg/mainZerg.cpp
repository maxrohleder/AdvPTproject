#include "../Zerg.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
    string a;
    cin >> a;
    Zerg z(a);
    int i = z.runTest(1000);
    cout << i << endl;
    return 0;
}