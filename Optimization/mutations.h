#include <string>
#include <list>
#include <map>
#include <iostream>
#include <vector>

#include "global_enums.h"

using namespace std;

// create n mutations
// cross breeding
// wenn zeit knapp drüber ist eine einheit entfernen
// mutation (zufällige mod an einer liste)
class Mutator{
    private:
    vector<string> *multiple = NULL;
    
    public:
    Mutator(){}
    Mutator(vector<string> *multi): multiple(multi){
         srand(3333);
    }
    Mutator(const Mutator &m){}
    ~Mutator(){}

    list<string> cross_breed(list<string> list1, list<string> list2){
        // TODO mix through two lists creating a new one
        // could be either equally long or shorter/longer, but should only be able 
        // to remove elements not beeing in multiple
        return list1;
    }

    list<string> mutate(list<string> list1){
        // TODO insert an item from multiple at a random position TOIMPLEMENT
        return list1;
    }


    void append_n_mutations(list<pair<list<string>, int>> &buildlists, int n){
        for(int i = 0; i < n; i++)
        {
            int chance = rand() % 2;
            if(chance == 0){
                // TODO randomly cross breed (and make this nicer :'D)
                int l1 = rand() % buildlists.size();
                int l2 = rand() % buildlists.size();
                auto list1 = buildlists.begin();
                advance(list1, l1);
                auto list2 = buildlists.begin();
                advance(list2, l2);
                
                list<string> quick1 = (*list1).first;

                list<string> quick2 = (*list2).first;

                buildlists.push_back(make_pair<list<string>, int>(cross_breed(quick1, quick2), UNTESTED));
            }
            else if(chance == 1){
                // TODO randomly add mutations
                int l1 = rand() % buildlists.size();
                auto list1 = buildlists.begin();
                advance(list1, l1);
                //buildlists->push_back(make_pair(mutate(list1), UNTESTED));
            }
        }
    }


}