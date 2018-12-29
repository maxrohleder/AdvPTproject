#include <iostream>
#include "parser.h"
using namespace std;

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "usage: " << argv[0] << "<techtree> <buildlistfile>" << endl;
        exit(-1);
    }
    cout << "reading in csv file" << endl; 
    // contains all dependency objects and a list of requested build strings.
    parser p(argv[1], argv[2]);
    list<string> seen;
    while(p.building()){
        
    }

    return 0;
}