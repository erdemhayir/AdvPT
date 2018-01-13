#pragma once

#include <vector>
#include <map>

using namespace std;

typedef map<string, int> Map;
typedef map<string, vector<int>> MapVec;
typedef Map::value_type Pair;

class Terran
{
public:
	// ---------- DECLARATIONS ---------- //
	map<string, int> buildList;			// All items with their corresponding mapNumbers, key = name, value = mapNumber
	map<string, int> initWorkerIDsMap;	// key = name, value = ID
	map<string, vector<int>> buildingIDsMap;	// key = name, value = ID
	int idNumber;						// responsible for 
	int increment;
	int idWorkerNumber;
	map<string, int> workerIDsMap;		// have to distinguish between initial workers and other workers added later during simulation, key = name, value = ID

	int buildDetails[34][10];			// All the details of items, read from .txt file
	int initWorkerIDs[6];				// element number -> ID of initial worker, contained values which specify items workers are working at

	vector<vector<int>> beingCreated;	// 2 columns : 1st column -> mapNumber, 2nd column -> time remained
	vector<int>			workerIDs;		// row number -> ID of worker, contained values which specify items workers are working at 
	vector<int>			finish;			// Collects all mapNubers whenever buildings are done (for 1 timestep)
	vector<int>			start;			// Collects all mapNubers whenever buildings are possible (for 1 timestep)

	// ---------- FUNCTIONS ------------- //
	Terran();
	void initMap();
	void initBuildDetails();
	void initIDs();
};