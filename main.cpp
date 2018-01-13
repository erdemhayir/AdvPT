#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "terran.h"
#include "json.h"
#include "simulation.h"
#include "parameter.h"

using namespace std;

int main(int argc, char const *argv[])
{
	Parameter param;
	Terran *terran	= new Terran();
	Simulation *sim = new Simulation();

	int takingTooLong = 0;

	ofstream json;
	json.open("stdout.json");
	json << "{\n \"buildlistValid\" : 1,\n \"game\": \"sc2-hots-terran\",\n \"initialUnits\": {\n \"scv\": [\n \"init_scv_00\",\n \"init_scv_01\",\n \"init_scv_02\",\n \"init_scv_03\",\n \"init_scv_04\",\n \"init_scv_05\"],\n \"command_center\": [\"init_command_center\"]},\n \"messages\" : [\n";
	json.close();

	string actualEvent;
	ifstream fin;
	fin.open("terran.txt");
	fin >> actualEvent;

	// ---------- Loop ------------- //
	do
	{
		terran->start.clear();
		sim->buildSuccess = 0;
		param.wasJsonWritten = 0;
		++takingTooLong;
		++param.Time;
		param.minerals += param.workersInMinerals * 0.7;
		param.vespene += param.workersInVespene * 0.35;

		if (param.energyCollect)
		{
			if (param.energy < 200) param.energy += 0.5625;
			else param.energy = 200;
		}

		if (takingTooLong == 73)
		{
			cout << "hi!" << endl;
		}

		// ---------- SIMULATION ------------- //
		sim->run(sim, terran, param, actualEvent, fin);

		OUT(terran, param);	// prints the details each step

		if (param.wasJsonWritten == 1) finishJson();
		if (param.wasJsonWritten == 2) finishJsonEnd();

	} while (!sim->error && takingTooLong < 1000);

	json.open("stdout.json", fstream::app);
	json << "]\n }";
	json.close();

	if (takingTooLong == 1000)
		sim->error = -1;		// Buildlist is invalid

	switch (sim->error)
	{
	case 1:
		stdoutput();			// Buildlist is successful. Output in stdout.json
		break;
	default:
		remove("stdout.json");	// Buildlist is invalid
		cout << "{ \"game\": \"sc2-hots-terran\",\n\"buildlistValid\" : 0 \n}";	
	}

	return 0;
}