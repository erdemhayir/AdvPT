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
    bool needVespene = false;


    //Now check skeleton for false case
    for (auto it = skeletonList.begin();it != skeletonList.end();++it){
        if(!it->second){
            mapNumber = it->first;

            int dependent = ob.buildDetails[mapNumber][9];
            int producedby = ob.buildDetails[mapNumber][8];

            if (ob.buildDetails[mapNumber][2] > 0 )
            {
                needVespene = true;
              /*  bool found = (std::find(skeletonList.begin(), skeletonList.end(), optPair (25, true)) != skeletonList.end());
                int count = std::count (skeletonList.begin(), skeletonList.end(), optPair (25, true));                }*/
            } else{
                needVespene = false;
            }

            std::random_device rd;     // only used once to initialise (seed) engine
            std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
            std::uniform_int_distribution<int> uni(0,3); // guaranteed unbiased

            auto random_worker = uni(rng);

            for (int i = 0; i < random_worker; ++i) {
                skeletonList.push_back(optPair(0, true));
                param.supplyUsed+=ob.buildDetails[0][4];
            }

            //Randomly assign Assimulator
            if (!needVespene)
            {
                int count = std::count (skeletonList.begin(), skeletonList.end(), optPair (25, true));
                if (count>2){
                    //DO Nothing
                }
                else if (count==1)
                {
                    std::uniform_int_distribution<int> uni_Ass(1,1);
                    auto random_assimulator = uni_Ass(rng);
                    for (int i = 0; i < random_assimulator; ++i) {
                        skeletonList.push_back(optPair(25, true));
                    }
                }

            } else{
                skeletonList.remove(optPair (25, true));


                    std::uniform_int_distribution<int> uni_Ass(1,2);
                    auto random_assimulator = uni_Ass(rng);

                    for (int i = 0; i < random_assimulator; ++i) {
                        skeletonList.push_back(optPair(25, true));
                    }



            }

            if (mapNumber<14){
                param.supplyUsed+=ob.buildDetails[mapNumber][4];
            }

            if (param.totalSupCost - param.supplyUsed /*- ob.buildDetails[mapNumber][4] */< 0)
            {
                skeletonList.push_back(optPair(15, true));
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

