#include <iostream>
#include "ValidatorZerg.h"
#include <string>

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Usage : " << argv[0] << " <flag> <inputfile>" << endl;
        exit(-1);
    }
    bool debug = false;
    bool valid = false;
    if(string(argv[1]).find('d') != string::npos){
        debug = true;
    }
    if(string(argv[1]).find('t') != string::npos){
        //create TerranChecker tc
        if (debug) //valid = tc.runDebug(argv[2]);
        //else valid = tc.run(argv[2]);
        if (valid) cout << "\nvalid" << endl;
        else cout << "\ninvalid" << endl;
        return 0;
    }
    if(string(argv[1]).find('z') != string::npos){
        ZergChecker zc;
        if(debug) valid = zc.runDebug(argv[2]);
        else valid = zc.run(argv[2]);
        if (valid) cout << "\nvalid" << endl;
        else cout << "\ninvalid" << endl;
        return 0; 
    }
    if(string(argv[1]).find('p') != string::npos){
        //create ProtossChecker pc
        if(debug) //valid = pc.runDebug(argv[2]);
        //else valid = pc.run(argv[2]);
        if (valid) cout << "\nvalid" << endl;
        else cout << "\ninvalid" << endl;
        return 0; 
    }
    cout << "invalid flag (try -d for debug and -t / -z / -p for Terran / Zerg / Protoss)" << endl;
    return -1;
}