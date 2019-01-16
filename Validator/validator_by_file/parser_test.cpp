#include "parser_test.h"
#include <iostream>

using namespace std;

int main(){
    parser p("../../../Validator/validator_by_file/techtree_zerg.csv", "", true);
    p.printMap();
    return 0;
}