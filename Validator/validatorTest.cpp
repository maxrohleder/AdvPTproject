#include <iostream>
#include "ValidatorZerg.h"
#include <string>

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Usage : " << argv[0] << " <flag> <inputfile>" << endl;
        exit(-1);
    }
    ZergChecker zc;
    bool valid = false;
    if(argc > 2){
        if(string(argv[1]) != "-d"){
            cout << "invalid flag (try -d)" << endl;
            exit(-1);
        }
        valid = zc.runDebug(argv[2]);
    }else{
        valid = zc.run(argv[1]);
    }
    if (valid) cout << "\nvalid" << endl;
    else cout << "\ninvalid" << endl;
    return 0; 
}