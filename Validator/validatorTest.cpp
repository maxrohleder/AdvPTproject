#include <iostream>
#include "ValidatorZerg.h"

int main(){
    cout << "here wtf" << endl;
    ZergChecker zc;
    cout << "init done" << endl;
    bool valid = zc.runDebug({"drone", "drone"});
    cout << valid << endl;
    return 0;
}