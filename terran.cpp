#include "terran.h"
#include <sstream>
#include <fstream>

Terran::Terran()	// Constructor
{
	vector<vector<int>> beingCreated(1, vector<int>(2, -1));	// Matrix (1 row, 2 columns) initialized with -1
	vector<int>			workerIDs;
	vector<int>			finish;
	
	initMap();	// First updates the map with all possible builds
	initBuildDetails();	// Then updates the matrix with the data from the file
	initIDs();
}

void Terran::initMap()
{
	// ---------- UNITS ---------- //
	buildList.insert(pair<string, int>("scv", 0));
	buildList.insert(pair<string, int>("marine", 1));
	buildList.insert(pair<string, int>("marauder", 2));
	buildList.insert(pair<string, int>("reaper", 3));
	buildList.insert(pair<string, int>("ghost", 4));
	buildList.insert(pair<string, int>("hellion", 5));
	buildList.insert(pair<string, int>("siege_tank", 6));
	buildList.insert(pair<string, int>("thor", 7));
	buildList.insert(pair<string, int>("medivac", 8));
	buildList.insert(pair<string, int>("viking", 9));
	buildList.insert(pair<string, int>("raven", 10));
	buildList.insert(pair<string, int>("banshee", 11));
	buildList.insert(pair<string, int>("battlecruiser", 12));

	// -------- BUILDINGS -------- //
	buildList.insert(pair<string, int>("command_center", 13));
	buildList.insert(pair<string, int>("orbital_command", 14));
	buildList.insert(pair<string, int>("planetary_fortress", 15));
	buildList.insert(pair<string, int>("refinery", 16));
	buildList.insert(pair<string, int>("engineering_bay", 17));
	buildList.insert(pair<string, int>("missile_turret", 18));
	buildList.insert(pair<string, int>("sensor_tower", 19));
	buildList.insert(pair<string, int>("barracks", 20));
	buildList.insert(pair<string, int>("factory", 21));
	buildList.insert(pair<string, int>("armory", 22));
	buildList.insert(pair<string, int>("bunker", 23));
	buildList.insert(pair<string, int>("ghost_academy", 24));
	buildList.insert(pair<string, int>("starport", 25));
	buildList.insert(pair<string, int>("fusion_core", 26));
	buildList.insert(pair<string, int>("supply_depot", 27));
	buildList.insert(pair<string, int>("barracks_with_reactor", 28));
	buildList.insert(pair<string, int>("barracks_with_tech_lab", 29));
	buildList.insert(pair<string, int>("starport_with_reactor", 30));
	buildList.insert(pair<string, int>("starport_with_tech_lab", 31));
	buildList.insert(pair<string, int>("factory_with_reactor", 32));
	buildList.insert(pair<string, int>("factory_with_tech_lab", 33));
};

void Terran::initIDs()
{
	initWorkerIDsMap.insert(pair<string, int>("init_scv_00", 0));
	initWorkerIDsMap.insert(pair<string, int>("init_scv_01", 1));
	initWorkerIDsMap.insert(pair<string, int>("init_scv_02", 2));
	initWorkerIDsMap.insert(pair<string, int>("init_scv_03", 3));
	initWorkerIDsMap.insert(pair<string, int>("init_scv_04", 4));
	initWorkerIDsMap.insert(pair<string, int>("init_scv_05", 5));
	buildingIDsMap.insert(pair<string, vector<int>>("init_command_center", { -1,-1 }));
	for (int i = 0; i < 6; ++i)
	{
		initWorkerIDs[i] = -1;
	}
}

void Terran::initBuildDetails()
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
	file.open("terranbuild.txt");
	while (file >> text)
	{
		buildDetails[i / 10][i % 10] = stoi(text);
		++i;
	}
}