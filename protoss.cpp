//
// Created by erdem on 19.11.17.
//

#include "protoss.h"
#include <sstream>
#include <fstream>
#include <iostream>

Protoss::Protoss()	// Constructor
{
    vector<vector<int>> beingCreated(1, vector<int>(2, -1));	// Matrix (1 row, 2 columns) initialized with -1
    vector<int>			workerIDs;
    vector<int>			finish;

    initMap();	// First updates the map with all possible builds
    initBuildDetails();	// Then updates the matrix with the data from the file
    initIDs();
}

void Protoss::initMap()
{


    // ---------- UNITS ---------- //
    buildList.insert(pair<string, int>("probe", 0));
    buildList.insert(pair<string, int>("zealot", 1));
    buildList.insert(pair<string, int>("stalker", 2));
    buildList.insert(pair<string, int>("sentry", 3));
    buildList.insert(pair<string, int>("warp_prism", 4));
    buildList.insert(pair<string, int>("immortal", 5));
    buildList.insert(pair<string, int>("observer", 6));
    buildList.insert(pair<string, int>("colossus", 7));
    buildList.insert(pair<string, int>("high_templar", 8));
    buildList.insert(pair<string, int>("dark_templar", 9));
    buildList.insert(pair<string, int>("mothership", 10));
    buildList.insert(pair<string, int>("phoenix", 11));
    buildList.insert(pair<string, int>("void_ray", 12));
    buildList.insert(pair<string, int>("carrier", 13));

    // -------- BUILDINGS -------- //

    buildList.insert(pair<string, int>("nexus", 14));
    buildList.insert(pair<string, int>("pylon", 15));
    buildList.insert(pair<string, int>("gateway", 16));
    buildList.insert(pair<string, int>("cybernetics_core", 17));
    buildList.insert(pair<string, int>("robotics_facility", 18));
    buildList.insert(pair<string, int>("robotics_bay", 19));
    buildList.insert(pair<string, int>("twilight_council", 20));
    buildList.insert(pair<string, int>("templar_archives", 21));
    buildList.insert(pair<string, int>("dark_shrine", 22));
    buildList.insert(pair<string, int>("stargate", 23));
    buildList.insert(pair<string, int>("fleet_beacon", 24));
    buildList.insert(pair<string, int>("assimilator", 25));
    buildList.insert(pair<string, int>("forge", 26));
    buildList.insert(pair<string, int>("photon_cannon", 27));
};

//TODO this code should be replace but I dont get it how to do it
void Protoss::initIDs()
{
    initWorkerIDsMap.insert(pair<string, int>("init_probe_00", 0));
    initWorkerIDsMap.insert(pair<string, int>("init_probe_01", 1));
    initWorkerIDsMap.insert(pair<string, int>("init_probe_02", 2));
    initWorkerIDsMap.insert(pair<string, int>("init_probe_03", 3));
    initWorkerIDsMap.insert(pair<string, int>("init_probe_04", 4));
    initWorkerIDsMap.insert(pair<string, int>("init_probe_05", 5));
    buildingIDsMap.insert(pair<string, int>("init_nexus", -1));
    for (int i = 0; i < 6; ++i)
    {
        initWorkerIDs[i] = -1;
    }
}

void Protoss::initBuildDetails()
{
    // Rows = Build (int from the map)
    // Column 0 = Quantity available
    // Column 1 = Minerals
    // Column 2 = Vespene
    // Column 3 = Build Time
    // Column 4 = Supply Cost TIMES TWO!!! (so it can be int)
    // Column 5 = Supply Provided
    // Column 6 = Start Energy
    // Column 7 = Max Energy
    // Column 8 = Produced by (int from map)
    // Column 9 = Dependency (int from map)

    int i = 0;
    string text;
    ifstream file;
    file.open("protossbuild.txt");




    while (file >> text)
    {
        //std::cout<<"Text is"<<text<<std::endl;
        buildDetails[i / 10][i % 10] = stoi(text);
        ++i;
    }
}




