#include <list>
#include <tuple>
#include <string>
#include <iostream>
#include <functional>
#include <string>

using namespace std;


class Race{
    protected:

	//printstruct
	struct printstruct{
		printstruct(string type, string name, string produced_id = "", string boosted_id = "") : type(type),
				 name(name), produced_id(produced_id), boosted_id(boosted_id){}
		printstruct(const printstruct& p) : type(p.type),
				 name(p.name), produced_id(p.produced_id), boosted_id(p.boosted_id){}
		~printstruct(){}
		bool operator!=(const printstruct& p) const {
			if(type == p.type && name == p.name & produced_id == p.produced_id && boosted_id == p.boosted_id){
				return false;
			}else{
				return true;
			}
		}
		string type;	//type of action
		string name;	//name of unit 
		string produced_id;		//id for built energy units 
		string boosted_id;		//id for unit getting boosted
	};

    //consts
    int minerals_rate = 70;
    int vesp_rate = 35;
	int energy_rate = 5625;
	string ability_trigger = "booooooooooost"; //string so terran can differ between building unit with id and activating boost

    //resources
    int minerals = 5000;
    int vespene = 0;
    int supply_used = 6;
    int supply_max = 20; //racedependent (change in constructor)

    //workers
    int workers = 6;
    int workers_minerals = 6;
    int workers_vesp = 0;
    int workers_vesp_max = 0;


    //list-structures
    
    list<printstruct> printlist;
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
		cout << "\t\t\t\t},\n\t\t\t\t\"resources\": {\n\t\t\t\t\t\"minerals\": " << (int)(minerals / 100) << "," << endl;
		cout << "\t\t\t\t\t\"vespene\": " << (int)(vespene / 100) << "," << endl;
		cout << "\t\t\t\t\t\"supply-used\": " << supply_used << "," << endl;
		cout << "\t\t\t\t\t\"supply\": " << supply_max << endl;
		cout << "\t\t\t\t}\n\t\t\t},\n\t\t\t\"events\": [" << endl;
		//alle events printen
		for (const auto& i : printlist) { 
			if(i.type == ""){ //IMPORTANT: put in the empty string pair at FRONT of the printlist
				continue;
			}
			cout << "\t\t\t\t{\n\t\t\t\t\t\"type\": \"" << i.type << "\"," << endl;
			cout << "\t\t\t\t\t\"name\": \"" << i.name << "\"";
			if(i.produced_id != ""){ 	//some kind of id needs to be set
				if(i.boosted_id == ""){		//id set for building a building
					cout << ",\n\t\t\t\t\t\"producedIDs\": [ \"" << i.produced_id << "\" ]";
				}else if(i.boosted_id == ability_trigger){		//id set for terran ability trigger
					cout << ",\n\t\t\t\t\t\"triggeredBy\": \"" << i.produced_id << "\"";
				}else{ 		//id set for zerg or protoss building boost
					cout << ",\n\t\t\t\t\t\"triggeredBy\": \"" << i.produced_id << "\"," << endl;
					cout << "\t\t\t\t\t\"targetBuilding\": \"" << i.boosted_id << "\"";
				}
			}
			
			if (i != printlist.back()) {
				cout << "\n\t\t\t\t},";
			}else{
				cout << "\n\t\t\t\t}";
			}
			cout << endl;
		}
		
		cout << "\t\t\t]\n\t\t},";
		//liste leeren
		printlist.clear();
	}
};
