Status:
- Layout structure done 
- Zerg implemented roughly done
- missing:
	- Protoss implementation
	- Terran a few functions
	- buildlist validator
	- energy management
	- id management
- pack project into project.zip (final)
	- should contain build.sh and forwardSim.sh <race> <buildlistFile>
	- sc2-hots-terran/protoss/zerg are valid args for <race>
	- print to stdout

Current Layout:
	
	Klasse Race:
		
		-enthaelt rassenunspezifische Variablen und Funktionen


	Klasse Terran, Protoss, Zerg:

		-erbt von Race
		-enthaelt rassenspezifische Variablen und Funktionen

	noetige Datenverwaltungsstrukturen:
		- buildlist (Liste aus strings)
		- eventlist (mapt zeiten t auf events)
	

class race{
protected:
	virtual function* name_function_mapping( string name ) {}; // aus race
	virtual update_resources();
	###
	all attributes of race
	###

	buildlist<strings, function>;	// baustart ereignisse
	eventlist<int, function>;		// ruft funktion zur beendigung eines ereignisses auf
	printlist<string, string>;		// beinhaltet strings die geprintet werden müssen
	void run(){
		for(	int i		){
			if(	zerg.build( buildlist.pop() )	){
				// in printlist einfügen
				// entzeitpunkt mit zugehöriger function in der eventlist erstellen
				builist.poll()
			}

			while ( find_if(eventlist == t) =! endptr ){
				eventlist[t].execute()
				printlist.add()
			}

			if(printlist.isnotempty()){
				print();
				printlist.clear();
			}
			update_resources();
		}
	}

}

class zerg : race {
public:
	zerg(string filename){
		// read in lines
		for( word in file){
			buildlist<function> = name_function_mapping(word);
		}
	}
protected:
	function* name_function_mapping( string name ) {};	
}







Klasse Race_stat:

	-enthaelt Daten, die fuer alle Rassen gleich sind


Klasse Race_func: //fuer Variante 1 und 2

	-enthaelt Funktionen, die fuer alle Rassen gleich sind
	-enthaelt event_list
	-enthaelt print()-Funktion fuer formatierten output


Variante 1:


	Klasse Zerg_stat, Terran_stat, Protoss_stat:

		-erbt von Race_stat
		-enthaelt rassenspezifische Daten
	

	Klasse Zerg_func, Terran_func, Protoss_func:

		-erbt von Race_func
		-enthaelt rassenspezifische Funktionen
		-enthaelt run()-Funktion


Variante 2:

	Klasse Zerg_stat:

		-enthaelt rassenspezifische Variablen


	Klasse Zerg, Terran, Protoss:
	
		-erbt von Race_stat, Race_func und Zerg_stat
		-einfacher, da die Funktionen auf Variablen zugreifen koennen


Variante 3:

	Klasse Race_func:

		-erbt von Race_stat (fuer Variablenzugriff)
		-enthaelt rassenunspezifische Funktionen


	Klasse Zerg_stat:

		-enthaelt rassenspezifische Variablen


	Klasse Zerg:

		-erbt von Race_func und Zerg_stat (fuer Zugriffe auf allg. Variablen/Funktionen und rassenspezifische Variablen)
		-enthaelt rassenspezifische Funktionen
