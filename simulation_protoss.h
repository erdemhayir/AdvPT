//
// Created by erdem on 24.12.17.
//

#ifndef OLEGPROJECT_SIMULATION_PROTOSS_H
#define OLEGPROJECT_SIMULATION_PROTOSS_H

#pragma once

#include "protoss.h"
#include "parameter.h"
#include <sstream>
#include <fstream>
#include <algorithm>

class Simulation_P
{
public:
    int error;
    int buildSuccess;
    int end;
    int over;

    Simulation_P() : error(0), buildSuccess(0), end(0), over(0) {};	// Constructor

    int createCurrentBuild(string actualEvent, Protoss &ob, Parameter &param);			// & is crucial here!------ because otherwise a copy of the object is created
    void addBeingCreated(int mapNumber, Protoss &ob, Parameter &param);					// & is crucial here!------ and after function finished its job
    void checkFinishedBuildings(Protoss &ob, Parameter &param);							// & is crucial here!------ the DESTRUCTOR of a copy will be called
    int checkCreated(Protoss ob);
    void writeJson(string type, int mapNumber, Protoss &ob, Parameter &param);			// & is crucial here!------ which will delete all the data!

    // collects all the functions above
    void run(Simulation_P *sim, Protoss *protoss, Parameter &param, string &actualEvent, ifstream &fin);
};

#endif //OLEGPROJECT_SIMULATION_PROTOSS_H
