//
// Created by erdem on 26.01.18.
//

#ifndef OLEGPROJECT_OPTIMIZATIONPROTOSS_H
#define OLEGPROJECT_OPTIMIZATIONPROTOSS_H


#include "simulation.h"
#include <random>
#include <list>
typedef std::pair<int, bool> optPair;

class Optimization {
private:
	std::list<std::string> skeleton_vector;
public:
	std::list<optPair> skeletonList;

    Optimization(){};
    void pushOptimization(Terran &ob, Parameter &param, std::string pushObject);
};


#endif //OLEGPROJECT_OPTIMIZATIONPROTOSS_H
