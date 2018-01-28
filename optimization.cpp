//
// Created by erdem on 26.01.18.
//

#include <iostream>
#include "optimization.h"

void Optimization::pushOptimization(Terran &ob, Parameter &param, std::string pushObject)
{
	Parameter param_temp = param;
    std::vector <int> tempSkeloton;
    int mapNumber = ob.buildList[pushObject];
    skeletonList.push_back(optPair(mapNumber, false));
	bool needVespene = false;

    for (auto it = skeletonList.begin();it != skeletonList.end();++it){
        if(!it->second){
            mapNumber = it->first;
            int dependent = ob.buildDetails[mapNumber][9];
            int producedby = ob.buildDetails[mapNumber][8];

			if (ob.buildDetails[mapNumber][2] > 0)	//refinery
			{
				needVespene = true;
			}
				/*bool flag = (std::find(skeletonList.begin(), skeletonList.end(), optPair(16, true)) != skeletonList.end());
				if (!flag)
					skeletonList.push_back(optPair(16, true)); 
					needVespene = true;*/
			else
			{
				/*skeletonList.remove(optPair(16, true));
				skeletonList.push_back(optPair(16, true));*/
				needVespene = false;
			}

			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(0, 3);

			auto random_worker = uni(rng);

			for (int i = 0; i < random_worker; ++i)
				skeletonList.push_back(optPair(0, true));

			//Randomly assign Assimulator
			if (!needVespene) {
				int count = std::count(skeletonList.begin(), skeletonList.end(), optPair(16, true));
				if (count > 2)
				{
					//DO Nothing
				}
				else if (count == 1)
				{
					std::uniform_int_distribution<int> uni_Ass(1, 1);
					auto random_assimulator = uni_Ass(rng);
					for (int i = 0; i < random_assimulator; ++i)
					{
						skeletonList.push_back(optPair(16, true));
					}
				}
			}
			else
			{
				skeletonList.remove(optPair(16, true));


				std::uniform_int_distribution<int> uni_Ass(1, 2);
				auto random_assimulator = uni_Ass(rng);

				for (int i = 0; i < random_assimulator; ++i)
					skeletonList.push_back(optPair(16, true));
			}

			/*if (mapNumber < 13) {
				param_temp.supplyUsed += ob.buildDetails[mapNumber][4];
			}

			if (param_temp.totalSupCost - param_temp.supplyUsed < 0)
			{
				skeletonList.push_back(optPair(27, true));
			}*/

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
    skeout.open("terran_temp.txt");

    for (auto it = skeletonList.crbegin(); it != skeletonList.crend(); ++it)
    {
        //std::cout<<it->first<<std::endl;
        mapNumber = it->first;
        Pair m = *find_if
                (ob.buildList.begin(), ob.buildList.end(),
                 [mapNumber](const Pair &p) { return p.second == mapNumber; }
                );
        skeout << m.first<<"\n";
    }
    skeout.close();

	param_temp = param;
	ifstream temp("terran_temp.txt");
	ofstream listout;
	listout.open("terran.txt");
	if (temp.is_open())
	{
		string actualEvent;
		while (temp >> actualEvent)
		{
			/*temp >> actualEvent;*/
			mapNumber = ob.buildList[actualEvent];

			if (mapNumber < 13) {
				param_temp.supplyUsed += ob.buildDetails[mapNumber][4];
			}
			if (param_temp.totalSupCost - param_temp.supplyUsed < 0)
			{
				listout << "supply_depot\n";
				param_temp.totalSupCost += 8;
			}

			listout << actualEvent << "\n";
		}
	}
	/*
	for (auto it = skeletonList.crbegin(); it != skeletonList.crend(); ++it)
	{
		//std::cout<<it->first<<std::endl;
		mapNumber = it->first;
		Pair m = *find_if
		(ob.buildList.begin(), ob.buildList.end(),
			[mapNumber](const Pair &p) { return p.second == mapNumber; }
		);
		skeout << m.first << "\n";
	}*/
	skeout.close();
}

