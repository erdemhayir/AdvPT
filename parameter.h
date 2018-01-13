#pragma once

// parameters for simulation
struct Parameter
{
	double	minerals = 50.0;
	double	vespene = 0.0;
	double	energy = 0.0;
	int		energyCollect = 0;
	int		totalSupCost = 10;
	int		supplyUsed = 6;
	int		workers = 6;
	int		Time = 0;
	int		workersInMinerals = workers;
	int		workersInVespene = 0;
	int		previousTime = -1;
	int		wasJsonWritten;
};