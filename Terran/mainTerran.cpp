#include <iostream>

#include "../Terran.h"

using namespace std;

int main(){
    string a;
    cin >> a;
    Terran ter(a);
    int i = ter.testRun(200000);
    cout << i << endl;
    return 0;
}
