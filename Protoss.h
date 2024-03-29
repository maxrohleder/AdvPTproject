#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <math.h>
#include <algorithm>
#include <fstream>

#include "Protoss/Protoss_status.h"
#include "Protoss/parser_protoss.h"

using namespace std;

enum sim_stat
{
    simulation_success = 0,
    simulation_timeout = 1,
    simulation_invalid = 2
};

// main class handling the forward of a buildlist
class Protoss : public Protoss_status
{
  private:
    string buildlistpath;
    string techtreepath;
    list<string> buildlist_strings;
    bool printToFile = false;
    ofstream *fileToWriteTo;

  protected:
    // helper functions
    void updateResources()
    {
        minerals += minerals_rate * workers_minerals;
        vespene += vesp_rate * workers_vesp;
        for (auto &nex : energylist)
        {
            nex.second = min(1000000, nex.second + energy_rate);
        }
    }

    void buildBuildlist(const string filename)
    {
        // first init buildmap
        initBuildmap();
        // then read in strings
        ifstream file;
        string unit_name;
        file.open(filename);
        if (file.is_open())
        {
            while (getline(file, unit_name))
            {
                if (buildmap.find(unit_name) == buildmap.end())
                {
                    // key does not exist. print invalid as json
                    cerr << "no unit named: <" << unit_name << "> in buildlist. aborting..." << endl;
                    exit(1);
                }
                buildlist.push_back(buildmap[unit_name]);
            }
            file.close();
        }
        else
        {
            cerr << "could not read file" << endl;
            exit(-1);
        }
    }

    void chronoBoost()
    {
        for (auto &nex : energylist)
        {
            if (debug)
                cout << "energy of nexus_" << to_string(nex.first) << ": " << nex.second << endl;
            if (nex.second >= 250000)
            {
                if (activebuildings.size() == 0)
                    continue;
                string building_to_be_boosted;
                // move some element in eventlist ( first try to find one that is active for at least 20s, if that fails just pick one randomly )
                list<end_event>::iterator i = find_if(eventlist.begin(), eventlist.end(), [this](const end_event p) { return ((find(activebuildings.begin(), activebuildings.end(), p.producerID) != activebuildings.end()) && !p.boosted && p.end_time > time && p.end_time - time >= 20 && p.type == "build-end"); });
                if (i == eventlist.end())
                {
                    // omit criterion of lasting at least 20 seconds from now
                    i = find_if(eventlist.begin(), eventlist.end(), [this](const end_event p) { return ((find(activebuildings.begin(), activebuildings.end(), p.producerID) != activebuildings.end()) && !p.boosted && p.end_time > time && p.type == "build-end"); });
                    if (i == eventlist.end())
                    {
                        // all active buildings are already boosted!
                        continue;
                    }
                }
                building_to_be_boosted = i->producerID;
                if (debug)
                    cout << "--------------------CHRONOBOOST----------------"
                         << "\non building: " << building_to_be_boosted << endl;
                // either we found one with the first search or the second, doesnt matter now
                boostedbuildings.push_back(building_to_be_boosted);
                // 50% faster means speedup by 2/3
                if (debug)
                    cout << "endtime: " << i->end_time << " time: " << time;
                double fac = (double)2 / (double)3;
                // TODO check if to ceil floor round etc
                i->end_time = time + (int)ceil(((double)i->end_time - (double)time) * fac);
                if (debug)
                    cout << " new endtime: " << i->end_time << endl;
                i->boosted = true;
                nex.second -= 250000;
                // add endevent to eventlist which moves endevent back ( true, so that we do not find a boosted endevent by accident )
                addToEventList(20, &Protoss_status::boostEnd, building_to_be_boosted, true, "chrono");
                // and print that shit
                addToPrintList("special", "chronoboost", "nexus_" + to_string(nex.first), building_to_be_boosted);
            }
        }
    }

    void revise_chrono()
    {
        // if we built something new, see if the building is boosted
        for (string b : boostedbuildings)
        {

            // if there is a building in eventlist, that is not boosted, but should be, boost it
            list<end_event>::iterator i = find_if(eventlist.begin(), eventlist.end(), [b](const end_event p) { return (p.producerID == b && !p.boosted && p.type == "build-end"); });
            if (i != eventlist.end())
            {
                i->boosted = true;
                double fac = (double)2 / (double)3;
                i->end_time = time + (int)ceil(((double)i->end_time - (double)time) * fac);
            }
        }
    }

    void updateEventlist()
    {
        while (1)
        {
            auto i = find_if(eventlist.begin(), eventlist.end(), [this](const end_event p) { return p.end_time == time; });
            if (i == eventlist.end())
            {
                return;
            }
            else
            {
                // invoking the funcVoids with their producerID. Only needed for boostEnd, so that the unit in building process is known
                (this->*(i->func))(i->producerID);
                eventlist.erase(i);
            }
        }
    }

    bool updateBuildlist()
    {
        if ((this->*(*buildlist.begin()))())
        {
            buildlist.pop_front();
            return true;
        }
        return false;
    }

    void printHeader()
    {
        sout << "{\n\t\"buildlistValid\": \"1\"," << endl;
        sout << "\t\"game\": \"sc2-hots-protoss\"," << endl;
        sout << "\t\"messages\": [" << endl;
    }

    /*does final printing from stringstream sout*/
    void printFinish(bool valid)
    {
        if (valid)
        {
            sout << "\t]," << endl;
            sout << "\"initialUnits\" : { \"probe\" : [ \"larry_der_fleissige\", \"1\", \"2\", \"3\", \"4\", \"5\" ],\n\"nexus\": [ \"nexus_0\" ] \n}\n}\n";
        }
        else
        {
            if (debug)
                cout << sout.str() << "\n\n\nnow real output:\n\n\n";
            sout.str("");
            sout << "{\n\t\"game\" : \"sc2-hots-protoss\",\n\t\"buildlistValid\" : \"0\"\n}" << endl;
        }
        if (printToFile)
        {
            if (fileToWriteTo != nullptr && fileToWriteTo->is_open())
            {
                *fileToWriteTo << sout.str() << std::endl;
            }
        }
        else
        {
            cout << sout.str();
        }
    }

    // checks only for hard dependencies and supply (timeout not detected)
    bool validateBuildlist()
    {
        if (techtreepath == "")
        {
            // if no techtree is specified, list was validated before
            return true;
        }

        if (buildlistpath != "")
        {
            parser p(techtreepath, buildlistpath, debug);
            return !validate(p);
        }
        else
        {
            parser p(techtreepath, buildlist_strings, debug);
            return !validate(p);
        }
    }

  public:
    Protoss(const list<string> buildlist_to_run, string techtree, bool dbg = false)
    {
        debug = dbg;
        techtreepath = techtree;
        buildlistpath = "";
        buildlist_strings = buildlist_to_run;
        initBuildmap();
        for (string item : buildlist_to_run)
        {
            buildlist.push_back(buildmap[item]);
        }
        supply_max = 10;
        idlebuildings.push_back("nexus_0");
        energylist.push_back(pair<int, int>(0, 0));
    }
    Protoss(const list<string> buildlist_to_run, bool dbg = false)
    {
        debug = dbg;
        techtreepath = "";
        buildlistpath = "";
        buildlist_strings = buildlist_to_run;
        initBuildmap();
        for (string item : buildlist_to_run)
        {
            buildlist.push_back(buildmap[item]);
        }
        supply_max = 10;
        idlebuildings.push_back("nexus_0");
        energylist.push_back(pair<int, int>(0, 0));
    }
    Protoss(const string filename, const string techtree = "", bool dbg = false)
    {
        debug = dbg;
        techtreepath = techtree;
        buildlistpath = filename;
        buildBuildlist(filename); // inits hashmap and fills it
        supply_max = 10;
        // initial buildings
        idlebuildings.push_back("nexus_0");
        energylist.push_back(pair<int, int>(0, 0));
    };

    Protoss(const Protoss &p)
    {
        cerr << "copy constructor not support\n";
        exit(1);
    };

    ~Protoss(){};

    void writeToFile(std::ofstream *file)
    {
        printToFile = true;
        fileToWriteTo = file;
    }

    int run(int endtime = 5000)
    {
        // print all invalid game
        if (!validateBuildlist())
        {
            printFinish(false);
            return simulation_invalid;
        }
        // print header for valid game
        printHeader();
        for (; time < endtime; ++time)
        {
            updateResources(); // 1. reevealuates resources and energy
            updateEventlist(); // 2. checks eventlist for events
            //checks if we can build some and if so increment revision
            if (!buildlist.empty())
            {
                if (updateBuildlist())
                {
                    // ideal worker distro has changed as we are now
                    // looking foreward to build something different
                    revision_requested = true;
                    revise_chrono();
                }
                else
                {
                    chronoBoost();
                }
            }
            // printing
            if (!printlist.empty())
            {
                print(time);
                if (buildlist.empty() && eventlist.empty())
                {
                    sout << endl;              // get rid of ,
                    printFinish(true);         // end json
                    return simulation_success; // simulation success
                }
                else
                {
                    sout << "," << endl;
                }
            }
        }
        // print invalid if timeout
        printFinish(false);
        return simulation_timeout; // timelimit reached; no successful simulation
    }

    int getEndTime(int maxtime = 5000)
    {
        for (; time < maxtime; ++time)
        {
            updateResources(); // 1. reevealuates resources
            updateEventlist(); // 2. checks eventlist for events
            if (!buildlist.empty())
            {
                if (updateBuildlist())
                {
                    revision_requested = true;
                    revise_chrono();
                }
                else
                {
                    chronoBoost();
                }
            }
            if (!printlist.empty())
            {
                print(time);
                if (buildlist.empty() && eventlist.empty())
                {
                    sout.clear();
                    return time;
                }
            }
        }
        return INT_MAX; // timelimit reached; no successful simulation
    }
};