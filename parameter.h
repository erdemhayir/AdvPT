#pragma once

// parameters for simulation
struct Parameter
{
	double	minerals = 50.0;
	double	vespene = 0.0;
	int		energyCollect = 0;
	int		totalSupCost = 11;
	int		supplyUsed = 6;
	int		workers = 6;
	int		Time = 0;
	int		workersInMinerals = workers;
	int		workersInVespene = 0;
	int		mule = 0;
	int		previousTime = -1;
	int		wasJsonWritten;
	int		increment = 1;
	int		idNumber;
	int		idInitWorker;
	int		maxTime = 1000;
};