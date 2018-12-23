#include <iostream>
#include "read_and_check.h"
using namespace std;

int main(int argc, char* argv[]){
    if(argc != 2){
        cout << "usage: " << argv[0] << "<filename>" << endl;
        exit(-1);
    }
    cout << "reading in csv file" << endl; 
    std::map< std::string, depObj*> techtree = parse_to_map(argv[1]);
    cout << "reading done.. checking name of nexus:\n";
    cout << techtree["nexus"]->name << "done something" << endl;
    return 0;
}