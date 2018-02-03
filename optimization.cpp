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

	std::random_device rd;
	std::mt19937 rng(rd());

    for (auto it = skeletonList.begin();it != skeletonList.end();++it){
        if(!it->second){
            mapNumber = it->first;
            int dependent = ob.buildDetails[mapNumber][9];
            int producedby = ob.buildDetails[mapNumber][8];

			std::uniform_int_distribution<int> uni(0, 3);

			auto random_worker = uni(rng);

			for (int i = 0; i < random_worker; ++i)
				skeletonList.push_back(optPair(0, true));

            if(dependent == -1){

            }
            else if (ob.buildDetails[dependent][0] ==0 )
            {
                tempSkeloton.push_back(dependent);
            }

            if (ob.buildDetails[producedby][0] == 0 && producedby != 0){
                tempSkeloton.push_back(producedby);
            }

            //For same level adding producedby and dependency at the same time
            for (auto it2 = tempSkeloton.begin();it2 != tempSkeloton.end();++it2){
                skeletonList.push_back(optPair(*it2, false));
            }

            tempSkeloton.clear();

            it->second = true;

        }
    }

	int vespene_position = 0,
		count = 0;
	
    for (auto it = skeletonList.crbegin(); it != skeletonList.crend(); ++it)
    {
        mapNumber = it->first;
        Pair m = *find_if
                (ob.buildList.begin(), ob.buildList.end(),
                 [mapNumber](const Pair &p) { return p.second == mapNumber; }
                );

		if (!vespene_position && ob.buildDetails[mapNumber][2] > 0)
			vespene_position = count;

		skeleton_vector.push_back(m.first);

		++count;
    }

	count = 0;
	param_temp = param;

	string actualEvent;
	int random_n_refinery = 0;
	int n_refinery = 0;
	if (vespene_position)
	{
		std::uniform_int_distribution<int> uni(1, 2);
		random_n_refinery = uni(rng);
	}

	auto it = skeleton_vector.begin();
	do
	{
		actualEvent = *it;
		mapNumber = ob.buildList[actualEvent];

		if (mapNumber < 13) {
			param_temp.supplyUsed += ob.buildDetails[mapNumber][4];
		}
		if (param_temp.totalSupCost - param_temp.supplyUsed < 0)
		{
			std::uniform_int_distribution<int> uni(0, count);
			auto random_supply_depot_position = uni(rng);

			int position = 0;
			auto it2 = skeleton_vector.begin();
			do
			{
				if (position == random_supply_depot_position)
				{
					skeleton_vector.insert(it2,"supply_depot");
					break;
				}

				++position;
				++it2;
			} while (it2 != skeleton_vector.end());

			param_temp.totalSupCost += 8;

			++count;
			++vespene_position;
		}

		if (random_n_refinery && vespene_position == count)
		{
			do // comment out do-while loop for 2 refineries one behind the other
			{
				std::uniform_int_distribution<int> uni(0, vespene_position);
				auto random_refinery_position = uni(rng);

				int position = 0;
				auto it2 = skeleton_vector.begin();
				do
				{
					if (position == random_refinery_position)
					{
						skeleton_vector.insert(it2, "refinery");
						/*if (random_n_refinery == 2)
						{
							skeleton_vector.insert(it2, "refinery");
						}*/
						break;
					}

					++position;
					++it2;
				} while (it2 != skeleton_vector.end());

				++n_refinery;

				++count;
				++vespene_position;
			} while (n_refinery < random_n_refinery);

			random_n_refinery = 0;
		}

		++count;
		++it;
	} while (it != skeleton_vector.end());
	
	ofstream listout;
	listout.open("terran.txt");

	for (auto it = skeleton_vector.begin(); it != skeleton_vector.end(); ++it)
	{
		listout << *it <<"\n";
	}
	listout.close();

}