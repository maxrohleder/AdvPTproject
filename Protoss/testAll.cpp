#include "../Protoss.h"
#include <iostream>
#include <string>
#include <fstream>  

using namespace std;

// test with ..\..\Protoss\advancedBuild.txt as argv[1]

int main(int argc, char *argv[]){
    Protoss P1("../../Protoss/protoss1.txt");
    Protoss P2("../../Protoss/protoss2.txt");
    Protoss P3("../../Protoss/protoss3.txt");
    
    std::ofstream of1 ("protoss1_results.txt");
    P1.writeToFile(&of1);
    int s1 = P1.run();
    of1.close();

    std::ofstream of2 ("protoss2_results.txt");
    P2.writeToFile(&of2);
    int s2 = P1.run();
    of2.close();

    std::ofstream of3 ("protoss3_results.txt");
    P3.writeToFile(&of3);
    of3.close();
    int s3 = P1.run();

    cout << "statuses: " + to_string(s1) + to_string(s2) + to_string(s3) << std::endl;
}
