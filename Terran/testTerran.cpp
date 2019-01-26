#include <iostream>

#include "parser_terran.h"
#include "../Terran.h"

using namespace std;

int main(int argc, char* argv[]){
    /*if(argc < 2){
        cout << "Usage : " << argv[0] << " <inputfile>" << "" << endl;
        exit(-1);
    }*/
    //Terran ter(argv[1]);
    //int i = ter.testRun(200000);
    parser p("../../Optimization/techtree_terran.csv", "../../Terran/terran3.txt");
    cout << validate(p,false);
    //cout << i << endl;
    return 0;
}
