#include <iostream>
#include <string>
#include <list>

#include "parser_terran.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 3){
        cout << "Usage : " << argv[0] << "<techtree> <inputfile>" << "" << endl;
        exit(-1);
    }
    string list = string(argv[2]);
    string tree = string(argv[1]);
    parser_terran p(tree, list);
    cout << validate(p) << endl;

    /* list<string> blist = {"scv", "scv", "command_center"};
    p.setBuildlist(blist); */
    //cout << validate(p) << endl;
    return 0;
}