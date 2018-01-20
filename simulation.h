#pragma once

#include "terran.h"
#include "parameter.h"
#include <sstream>
#include <fstream>
#include <algorithm>

class Simulation
{
public:
	int error;
	int buildSuccess;
	int specialSuccess;
	int end;
	
	Simulation() : error(0), buildSuccess(0), end(0) {};	// Constructor

	int createCurrentBuild(string actualEvent, Terran &ob, Parameter &param);			// & is crucial here!------ because otherwise a copy of the object is created 
	void addBeingCreated(int mapNumber, Terran &ob, Parameter &param);					// & is crucial here!------ and after function finished its job 
	void checkFinishedBuildings(Terran &ob, Parameter &param);							// & is crucial here!------ the DESTRUCTOR of a copy will be called
	int checkCreated(Terran ob);
	void writeJson(string type, int mapNumber, Terran &ob, Parameter &param);			// & is crucial here!------ which will delete all the data!
	int specialEvent(Terran &ob, Parameter &param);

	// collects all the functions above
	void run(Simulation *sim, Terran *terran, Parameter &param, string &actualEvent, ifstream &fin);
};
