#pragma once
#include <iostream>
#include <string>
#include "list_builder.h"

int main(int argc, char* argv[]){
    if(argc < 3){
        cout << "Usage: " << string(argv[0]) << " <path to techtree> <unit to build>" << endl;
        exit(1);
    }
    list_builder lb(string(argv[1]), 'z');
    lb.runDebug(string(argv[2]));
    return 0;
}