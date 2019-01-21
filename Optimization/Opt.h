#include <string>
#include <list>
#include <map>
#include <iostream>
#include "list_builder.h"
#include "../Zerg.h"
#include "../Terran.h"
#include "../Protoss.h"
#include "../Validator/ValidatorZerg.h"
#include "../Validator/ValidatorTerran.h"

using namespace std;

class Opt{
        Opt(string tech_tree, string target, int amount, bool rush = true){}
        Opt(const Opt& o){}
        ~Opt(){}

    private:
        

};