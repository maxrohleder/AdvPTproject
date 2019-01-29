#include <iostream>
#include <string>
#include <list>

#include "parser_protoss.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 3  || argc > 4){
        cout << "Usage : " << argv[0] << "<techtree> <inputfile> (<debug>)" << "" << endl;
        exit(-1);
    }
    string list = string(argv[2]);
    string tree = string(argv[1]);
    bool debug = false;
    if(argc == 4) debug = (stoi(string(argv[3])) == 1);
    parser p(tree, list, debug);
    cout << validate(p, debug) << endl;

    /* list<string> blist = {"scv", "scv", "command_center"};
    p.setBuildlist(blist); 
    cout << validate(p) << endl;*/
    return 0;
}
