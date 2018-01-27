//
// Created by erdem on 26.01.18.
//

#include <iostream>
#include "optimizationProtoss.h"


void OptimizationProtoss::pushOptimization(Protoss &ob, Parameter &param, std::string pushObject){


    std::list<optPair> skeletonList;
    std::vector <int> tempSkeloton;
    int mapNumber = ob.buildList[pushObject];
    skeletonList.push_back(optPair(mapNumber, false));



    //Now check skeleton for false case
    for (auto it = skeletonList.begin();it != skeletonList.end();++it){
        if(!it->second){
            mapNumber = it->first;

            int dependent = ob.buildDetails[mapNumber][9];
            int producedby = ob.buildDetails[mapNumber][8];

            if (ob.buildDetails[mapNumber][2] > 0 )
            {
                skeletonList.push_back(optPair(25, true)); //assimulator
            }


            if(dependent == -1){

            }
            else if (ob.buildDetails[dependent][0] ==0 )
            {
                tempSkeloton.push_back(dependent);
            }

            if (ob.buildDetails[producedby][0] == 0 && producedby != 0){
                tempSkeloton.push_back(producedby);
                //skeletonList.push_back(producedby);
            }

            //For same level adding producedby and dependency at the same time
            for (auto it2 = tempSkeloton.begin();it2 != tempSkeloton.end();++it2){
                skeletonList.push_back(optPair(*it2, false));
            }

            tempSkeloton.clear();

            it->second = true;

        }
    }


    ofstream skeout;
    skeout.open("protoss4.txt");

    for (auto it = skeletonList.crbegin(); it != skeletonList.crend(); ++it)
    {
        std::cout<<it->first<<std::endl;
        mapNumber = it->first;
        Pair m = *find_if
                (ob.buildList.begin(), ob.buildList.end(),
                 [mapNumber](const Pair &p) { return p.second == mapNumber; }
                );
        skeout << m.first<<"\n";

    }
    skeout.close();








        /*Pair m = *find_if
                (ob.buildList.begin(), ob.buildList.end(),
                 [mapNumber](const Pair &p) { return p.second == mapNumber; }
                );
        ob.buildDetails[mapNumber][9]*/

    }

