#include <iostream>
#include "ValidatorZerg.h"

int main(){
    ZergChecker zc;
    bool valid = zc.runDebug({"drone", "drone"});
    cout << valid << endl;
    return 0; 
}