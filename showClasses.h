#include <list>
#include <tuple>
#include <string>
#include <iostream>

using namespace std;


class Race{
	//rassenunspezifische Variablen
protected:

	//rassenunspezifische Funktionen
protected:
	list<pair<string, string>> event_list; //list fuer alles, was in diesem Zeitschritt passiert (alte print_list)
	void print(int time, int workers_m, int workers_v,int minerals, int vespene, int sup_used, int sup) {
		//mit allen uebergebenen Parametern printen
		//liste entsprechend printen
		cout << "\t\t{" << endl;
		cout << "\t\t\t\"time\": " << time << "," << endl;
		cout << "\t\t\t\"status\": {\n\t\t\t\t\"workers\": {\n\t\t\t\t\t\"minerals\": " << workers_m << "," << endl;
		cout << "\t\t\t\t\t\"vespene\": " << workers_v << endl;
		cout << "\t\t\t\t},\n\t\t\t\t\"recources\": {\n\t\t\t\t\t\"minerals\": " << minerals << "," << endl;
		cout << "\t\t\t\t\t\"vespene\": " << vespene << "," << endl;
		cout << "\t\t\t\t\t\"supply-used\": " << sup_used << "," << endl;
		cout << "\t\t\t\t\t\"supply\": " << sup << endl;
		cout << "\t\t\t\t}\n\t\t\t},\n\t\t\t\"events\": [" << endl;
		//alle events printen
		for (const auto& i : event_list) { 
			cout << "\t\t\t\t{\n\t\t\t\t\t\"type\": " << i.first << "," << endl;
			cout << "\t\t\t\t\t\"name\": " << i.second << "\n\t\t\t\t}";
			if ( i != *event_list.end()) {
				cout << ",";
			}
			cout << endl;
		}
		
		cout << "\t\t\t]\n\t\t}," << endl;
		//liste leeren
		event_list.clear();
	}
};

//Bsp. Zerg


//hier evtl von beiden erben
class Zerg : public Race {
	//rassenspezifische Variablen
private:
	//rassenspezifische Methoden
public:
	//run()-Methode
	Zerg(string file_name){
		//file in build_list lesen
	}
	Zerg(const Zerg& a) {}
	~Zerg() {}
	void run() {
		for (int i = 1; i < 360; ++i) {
			//Recourcen erh�hen
			//etwas fertig geworden? -> in event_list einf�gen
			//was neues bauen? -> in event_list einf�gen
			//alles was passiert ist printen
			print(0, 0, 0, 0, 0, 0, 0); //event_list wird nach printen geleert
		}

	}
};
