#include <iostream>

#include "../Terran.h"

using namespace std;

int main(){
    string a;
    cin >> a;
    Terran ter(a);
    return ter.run();
}
