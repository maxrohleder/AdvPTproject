#include <iostream>
#include "read_and_check.h"
using namespace std;

int main(int argc, char* argv[]){
    if(argc != 2){
        cout << "usage: " << argv[0] << "<filename>" << endl;
        exit(-1);
    }
    cout << "reading in csv file" << endl; 
    validator_byfile val(argv[1]);
    //val.test_parse(argv[1]);

    //cout << val.test["5"] << endl;


    cout << "reading done.. checking name of name:\n";
    val.set_obj("nexus", -10, "probe", "");
    depObj* test = val.get_obj("nexus");
    cout << test->name << " success" << endl;
    val.print("nexus");
    val.print("wrong_test"); 

    return 0;
}