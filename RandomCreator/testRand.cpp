#include <iostream>
#include "randomCreator.h"
#include <list>
#include <string>

int main(int argc, char* argv[]){
    list<string> l;
    l = createRandomZergList(30);
    for(auto i : l){
        cout << i << ", ";
    }
    cout << endl;
    return 0;
}
