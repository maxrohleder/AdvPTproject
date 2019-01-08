#include <iostream>
#include "parser.h"
#include <algorithm>

using namespace std;

// test with: .\validate.exe ..\..\..\Validator\validator_by_file\techtree_protoss.csv ..\..\..\Validator\validator_by_file\advancedBuild.txt 
// from within build\Validator\validator_by_file

// returns 0 on successfull buildlist 1 otherwise
int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "usage: " << argv[0] << " <techtree> <buildlistfile>" << endl;
        exit(-1);
    }
    int status = validate(argv[1], argv[2]);
    if(status == 0) cout << "buildlist valid";
    else cout << "buildlist invalid";
}