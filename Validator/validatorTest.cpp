#include <iostream>
#include "ValidatorZerg.h"

int main(){
    ZergChecker zc;
    bool valid = zc.runDebug({"drone", "drone"});
    if (valid) cout << "valid" << endl;
    else cout << "invalid" << endl;
    return 0; 
}