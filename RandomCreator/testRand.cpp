#include <iostream>
#include "randomCreator.h"
#include <list>
#include <string>
#include "../Validator/ValidatorZerg.h"

int main(int argc, char* argv[]){
    list<string> l;
    ZergChecker zc;
    /*for(auto i : l){
        cout << i << endl;;
    }*/
    for(int i = 0;i < 100;){
        if(zc.run(createRandomZergList(5))) cout << ++i << endl;
    }
    return 0;
}
