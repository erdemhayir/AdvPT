#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "terran.h"
#include "json.h"
#include "simulation.h"
#include "parameter.h"
#include "optimization.h"
#include <chrono>

using get_time = chrono::steady_clock;
typedef std::chrono::time_point<std::chrono::steady_clock> timer;
typedef std::chrono::duration<float> fsec;

using namespace std;

int main(int argc, char const *argv[])
{
	Parameter param;

	std:string objectName = "viking";

	const timer start = get_time::now();
	timer end;
	fsec diff;

	int	minTime = param.maxTime;
	std::list<optPair> finalBuildlist;

	do
	{
		Parameter temp_param = param;

		Terran *terran = new Terran();
		Simulation *sim = new Simulation();
		Json json_ob;

		Optimization *opt = new Optimization();

		opt->pushOptimization(*terran, temp_param, objectName);
		sim->simulation(sim, terran, temp_param, json_ob);

		if (temp_param.totalTime < minTime)
		{
			minTime = temp_param.totalTime;
			finalBuildlist = opt->skeletonList;
		}

		end = get_time::now();
		diff = end - start;
	}while (diff.count() < 10.0);
	
	{
		Terran *terran = new Terran();
		Simulation *sim = new Simulation();
		Json json_ob;

		ofstream skeout;
		skeout.open("terran.txt");

		for (auto it = finalBuildlist.crbegin(); it != finalBuildlist.crend(); ++it)
		{
			//std::cout << it->first << std::endl;
			int mapNumber = it->first;
			Pair m = *find_if
			(terran->buildList.begin(), terran->buildList.end(),
				[mapNumber](const Pair &p) { return p.second == mapNumber; }
			);
			skeout << m.first << "\n";

		}
		skeout.close();

		sim->simulationFinal(sim, terran, param, json_ob);

		//json_ob.stdoutput();
	}

	system("pause");

	return 0;
	
}