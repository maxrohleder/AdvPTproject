#include "../Protoss.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
    string a;
    cin >> a;
    Protoss P(a);
    int i = P.runTest(1000);
    cout << i << endl;
    return 0;
}