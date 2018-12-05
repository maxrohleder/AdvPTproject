#include <list>
#include <tuple>
#include <string>
#include <iostream>
#include <functional>

using namespace std;

class Race{
    protected:
    //consts
    int minerals_rate = 70;
    int vesp_rate = 35;

    //resources
    int minerals = 5000;
    int vespene = 0;
    int supply_used = 6;
    int supply_max = 0; //racedependent

    //workers
    int workers = 6;
    int workers_minerals = 6;
    int workers_vesp = 0;
    int workers_vesp_max = 0;


    //list-structures
    list<function<bool(void)>> buildlist;
    list<pair<int, function<void(void)>>> eventlist;
    list<pair<string, string>> printlist;
    list<pair<int, int>> energylist;



    //structures
    int bases = 1;
    int geyser_max = 2;

    //raceindependent functions

    void print(int time) {
		//mit allen uebergebenen Parametern printen
		//liste entsprechend printen
		cout << "\t\t{" << endl;
		cout << "\t\t\t\"time\": " << time << "," << endl;
		cout << "\t\t\t\"status\": {\n\t\t\t\t\"workers\": {\n\t\t\t\t\t\"minerals\": " << workers_minerals << "," << endl;
		cout << "\t\t\t\t\t\"vespene\": " << workers_vesp << endl;
		cout << "\t\t\t\t},\n\t\t\t\t\"recources\": {\n\t\t\t\t\t\"minerals\": " << minerals << "," << endl;
		cout << "\t\t\t\t\t\"vespene\": " << vespene << "," << endl;
		cout << "\t\t\t\t\t\"supply-used\": " << supply_used << "," << endl;
		cout << "\t\t\t\t\t\"supply\": " << supply_max << endl;
		cout << "\t\t\t\t}\n\t\t\t},\n\t\t\t\"events\": [" << endl;
		//alle events printen
		for (const auto& i : printlist) { 
			cout << "\t\t\t\t{\n\t\t\t\t\t\"type\": " << i.first << "," << endl;
			cout << "\t\t\t\t\t\"name\": " << i.second << "\n\t\t\t\t}";
			if ( i != *printlist.end()) {
				cout << ",";
			}
			cout << endl;
		}
		
		cout << "\t\t\t]\n\t\t}," << endl;
		//liste leeren
		printlist.clear();
	}
};