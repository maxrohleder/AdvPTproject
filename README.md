Current Layout:
	
	Klasse Race:
		
		-enthaelt rassenunspezifische Variablen und Funktionen


	Klasse Terran, Protoss, Zerg:

		-erbt von Race
		-enthaelt rassenspezifische Variablen und Funktionen


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
