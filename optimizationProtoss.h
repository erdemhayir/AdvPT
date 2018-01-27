//
// Created by erdem on 26.01.18.
//

#ifndef OLEGPROJECT_OPTIMIZATIONPROTOSS_H
#define OLEGPROJECT_OPTIMIZATIONPROTOSS_H


#include "simulation_protoss.h"
#include <list>
typedef std::pair<int, bool> optPair;

class OptimizationProtoss {


public:
    OptimizationProtoss(){};
    void pushOptimization(Protoss &ob, Parameter &param, std::string pushObject);
};


#endif //OLEGPROJECT_OPTIMIZATIONPROTOSS_H
