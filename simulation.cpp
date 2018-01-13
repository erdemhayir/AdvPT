#include "simulation.h"


//	createCurrentBuild	: building is NOT possible	-> returns 0 
//						: building is possible		-> returns 1 
int Simulation::createCurrentBuild(string actualEvent, Terran &ob, Parameter &param)
{
	if (ob.start.size() > 0)
	{
		//only one unit is possible to start to build at one timestep
		return 0;
	}

	//mapnumber = terranBuilds["string"]
	int mapNumber = ob.buildList[actualEvent];

	if (param.totalSupCost - param.supplyUsed - ob.buildDetails[mapNumber][4] < 0)
	{
		//cout<<"supply2"<<endl;
		return 0;
	}

	int dependent = ob.buildDetails[mapNumber][9];
	if (dependent == -1)
	{
		// No dependency, we do nothing
	}
	else if (dependent == 20)
	{ // 20 or 28 or 29
		if ((ob.buildDetails[20][0] == 0) && (ob.buildDetails[28][0] == 0) && (ob.buildDetails[29][0] == 0)) {/*cout<<"dependency"<<endl;*/ return 0; }
	}
	else if (dependent == 21) { // 21 or 32 or 33
		if ((ob.buildDetails[21][0] == 0) && (ob.buildDetails[32][0] == 0) && (ob.buildDetails[33][0] == 0)) {/*cout<<"dependency"<<endl;*/ return 0; }
	}
	else if (dependent == 25) { // 25 or 30 or 31
		if ((ob.buildDetails[25][0] == 0) && (ob.buildDetails[30][0] == 0) && (ob.buildDetails[31][0] == 0)) {/*cout<<"dependency"<<endl;*/ return 0; }
	}
	else if (ob.buildDetails[dependent][0] == 0) {/*cout<<"dependency"<<endl;*/ return 0; }

	if (ob.buildDetails[mapNumber][1] > param.minerals || ob.buildDetails[mapNumber][2] > param.vespene)
	{
		//cout<<"resource"<<endl;
		return 0;
	}

	if (mapNumber == 16)	// refinery limitation
	{
		string key = "refinery";
		
		MapVec::iterator it = ob.buildingIDsMap.begin();
		MapVec::iterator itend = ob.buildingIDsMap.end();
		int quantity = 0;
		do
		{
			string dec = it->first;
			dec.erase(dec.size() - 3, 3);

			if (dec == key)
			{
				++quantity;
			}
			++it;
		} while (it != itend);
		if (quantity == 2)
		{
			return 0;
		}
	}

	int producedby = ob.buildDetails[mapNumber][8];
	//for units
	if (mapNumber > 0 && mapNumber < 13 /* && ob.beingCreated.size() > 0*/)
	{
		Pair m = *find_if
		(ob.buildList.begin(), ob.buildList.end(), [producedby](const Pair &p)
		{	return p.second == producedby;	}
		);

		string producedbyName = m.first;

		MapVec::iterator it = ob.buildingIDsMap.begin();
		MapVec::iterator itend = ob.buildingIDsMap.end();
		do
		{
			string dec = it->first;
			dec.erase(dec.size() - 3, 3);

			if (it->first == "init_command_center" && ob.buildingIDsMap.find(it->first)->second[0] == -1)
			{
				break;
			}

			if (dec == producedbyName && it->second[0] == -1)
			{
				break;
			}
			++it;
		} while (it != itend);
		if (it == itend)
		{
			return 0;
		}
	}

	if ((ob.buildDetails[producedby][0] == 0) && producedby != 13 && producedby != 20 && producedby != 21 && producedby != 25)
	{
		//cout<<"producedby"<<endl;
		return 0;
	}
	else
	{
		if (producedby == 0) //produced by SCV (worker)
		{
			int unsigned j = -1;
			do
			{
				++j;
			} while (ob.initWorkerIDs[j] != -1 && (j <= sizeof(ob.initWorkerIDs) / sizeof(int) - 1));
			if (j < 6)
			{
				ob.initWorkerIDs[j] = mapNumber;
				param.idNumber = j;
			}
			else
			{
				int unsigned h = -1;
				do
				{
					++h;
				} while (ob.workerIDs[h] != -1 && (h <= ob.workerIDs.size() - 1));
				if (h == ob.workerIDs.size() - 1)
				{
					return -1;	//no worker available 
				}
				else ob.workerIDs[h] = mapNumber;
			}
			--param.workersInMinerals;
		}
		else if (producedby == 13 && mapNumber == 0)	// SCV
		{
			string key = "init_command_center";

			MapVec::iterator itend = ob.buildingIDsMap.end(); //--it;
			MapVec::iterator it = ob.buildingIDsMap.begin(); //--itbegin;
			do
			{
				string dec = it->first;

				if (dec == key && it->second[0] == -1)
				{
					break;
				}
				++it;
			} while (it != itend);				//"init_command_center"
			if (it != itend)
			{
				it->second[0] = mapNumber;
				param.idNumber = 99;
			}
			else
			{
				string key = "orbital_command";

				MapVec::iterator it = ob.buildingIDsMap.end(); --it;
				MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					if (dec == key && it->second[0] == -1 && it->second[1] < 1)
					{
						break;
					}
					--it;
				} while (it != itbegin);
				if (it != itbegin)
				{
					it->second[0] = mapNumber;
					param.idNumber = stoi(it->first.substr(it->first.length() - 2));
				}
				else
				{
					string key = "command_center";

					MapVec::iterator it = ob.buildingIDsMap.end(); --it;
					MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
					do
					{
						string dec = it->first;
						dec.erase(dec.size() - 3, 3);

						if (dec == key && it->second[0] == -1 && it->second[1] < 1)
						{
							break;
						}
						--it;
					} while (it != itbegin);
					if (it != itbegin)
					{
						it->second[0] = mapNumber;
						param.idNumber = stoi(it->first.substr(it->first.length() - 2));
					}
					else
					{
						string key = "planetary_fortress";

						MapVec::iterator it = ob.buildingIDsMap.end(); --it;
						MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 3, 3);

							if (dec == key && it->second[0] == -1 && it->second[1] < 1)
							{
								break;
							}
							--it;
						} while (it != itbegin);
						if (it != itbegin)
						{
							it->second[0] = mapNumber;
							param.idNumber = stoi(it->first.substr(it->first.length() - 2));
						}
						else return 0;
					}
				}
			}
		}
		else if (producedby == 13)	// command_center_upgraded
		{
			string key = "init_command_center";
			
			MapVec::iterator itend = ob.buildingIDsMap.end(); //--it;
			MapVec::iterator it = ob.buildingIDsMap.begin(); //--itbegin;
			do
			{
				string dec = it->first;

				if (dec == key && it->second[0] == -1)
				{
					break;
				}
				++it;
			} while (it != itend);				//"init_command_center"
			if (it != itend)
			{
				it->second[0] = mapNumber;
				param.idNumber = 99;
				/*param.idNumber = stoi(it->first.substr(it->first.length() - 2));*/
			}
			else
			{
				string key = "command_center";

				MapVec::iterator it = ob.buildingIDsMap.end(); --it;
				MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					if (dec == key && it->second[0] == -1)
					{
						break;
					}
					--it;
				} while (it != itbegin);
				if (it != itbegin)
				{
					it->second[0] = mapNumber;
					param.idNumber = stoi(it->first.substr(it->first.length() - 2));
				}
				else return 0;
			}
		}
		else
		{
			Pair m = *find_if
			(ob.buildList.begin(), ob.buildList.end(), [producedby](const Pair &p)
			{	return p.second == producedby;	}
			);

			string name = m.first;

			MapVec::iterator it = ob.buildingIDsMap.end(); --it;
			MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
			do
			{
				string dec = it->first;
				dec.erase(dec.size() - 3, 3);

				if (dec == name && it->second[0] == -1 && it->second[1] < 1)
				{
					if ((producedby == 20 || producedby == 21 || producedby == 25)
						&& ob.buildDetails[producedby][0] == 0)
					{
					}
					else
					{
						it->second[0] = mapNumber;
						param.idNumber = stoi(it->first.substr(it->first.length() - 2));
						break;
					}
				}
				--it;
			} while (it != itbegin);
			if (it == itbegin)
			{
				if (producedby != 20 && producedby != 21 && producedby != 25)
					return 0;
				else
				{
					if (producedby == 20)
					{
						string name = "barracks_with_reactor";

						MapVec::iterator it = ob.buildingIDsMap.end(); --it;
						MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 3, 3);

							if (dec == name && it->second[0] == -1 && it->second[1] < 1)
							{
								it->second[0] = mapNumber;
								param.idNumber = stoi(it->first.substr(it->first.length() - 2));
								break;
							}
							--it;
						} while (it != itbegin);
						if (it != itbegin)
							goto stop;

						name = "barracks_with_reactor_00";

						it = ob.buildingIDsMap.end(); --it;
						itbegin = ob.buildingIDsMap.begin(); --itbegin;

						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 1, 1);

							if (dec == name && it->second[0] == -1 && it->second[1] < 1)
							{
								it->second[0] = mapNumber;
								param.idNumber = stoi(it->first.substr(it->first.length() - 2));
								break;
							}
							--it;
						} while (it != itbegin);
						if (it != itbegin)
							goto stop;
						else
							return 0;

					stop:
						{}
					}
					else if (producedby == 21)
					{
						string name = "factory_with_reactor";

						MapVec::iterator it = ob.buildingIDsMap.end(); --it;
						MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 3, 3);

							if (dec == name && it->second[0] == -1 && it->second[1] < 1)
							{
								it->second[0] = mapNumber;
								param.idNumber = stoi(it->first.substr(it->first.length() - 2));
								break;
							}
							--it;
						} while (it != itbegin);
						if (it != itbegin)
							goto stop2;

						name = "factory_with_reactor_00";

						it = ob.buildingIDsMap.end(); --it;
						itbegin = ob.buildingIDsMap.begin(); --itbegin;

						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 1, 1);

							if (dec == name && it->second[0] == -1 && it->second[1] < 1)
							{
								it->second[0] = mapNumber;
								param.idNumber = stoi(it->first.substr(it->first.length() - 2));
								break;
							}
							--it;
						} while (it != itbegin);
						if (it != itbegin)
							goto stop2;
						else
							return 0;

					stop2:
						{}
					}
					else if (producedby == 25)
					{
						string name = "starport_with_reactor";

						MapVec::iterator it = ob.buildingIDsMap.end(); --it;
						MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 3, 3);

							if (dec == name && it->second[0] == -1 && it->second[1] < 1)
							{
								it->second[0] = mapNumber;
								param.idNumber = stoi(it->first.substr(it->first.length() - 2));
								break;
							}
							--it;
						} while (it != itbegin);
						if (it != itbegin)
							goto stop3;

						name = "starport_with_reactor_00";

						it = ob.buildingIDsMap.end(); --it;
						itbegin = ob.buildingIDsMap.begin(); --itbegin;

						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 1, 1);

							if (dec == name && it->second[0] == -1 && it->second[1] < 1)
							{
								it->second[0] = mapNumber;
								param.idNumber = stoi(it->first.substr(it->first.length() - 2));
								break;
							}
							--it;
						} while (it != itbegin);
						if (it != itbegin)
							goto stop3;
						else
							return 0;

					stop3:
						{}
					}
				}
			}			
		}

		addBeingCreated(mapNumber, ob, param);

		return 1;
	}
}

//	addBeingCreated		: called in "createCurrentBuild" function when building is possible
void Simulation::addBeingCreated(int mapNumber, Terran &ob, Parameter &param)
{
	int unsigned i = 0;
	while ((i != ob.beingCreated.size()) && (ob.beingCreated[i][0] != -1)) ++i;

	if (i == ob.beingCreated.size())
	{
		ob.beingCreated.push_back(vector<int>(2, -1));
	}

	ob.beingCreated[i][0] = mapNumber;
	ob.beingCreated[i][1] = ob.buildDetails[mapNumber][3];	// remaining time added

	param.minerals -= ob.buildDetails[mapNumber][1];
	param.vespene -= ob.buildDetails[mapNumber][2];
	param.supplyUsed += ob.buildDetails[mapNumber][4];

	ob.start.push_back(mapNumber);

	if (mapNumber == 0)				//	SCV
	{
		int j = 0;
		if (ob.workerIDsMap.size() == 0)
			j = 0;
		else
		{
			MapVec::iterator it = ob.workerIDsMap.end();
			--it;
			j = 1 + stoi(it->first.substr(it->first.length() - 2));
		}
		string str = "";
		if (j > 9)
		{
			str = "scv_";
		}
		else
		{
			str = "scv_0";
		}
		str += to_string(j);
		ob.workerIDsMap.insert(pair<string, vector<int>>(str, { param.idNumber, param.increment }));
	}
	else
	{
		Pair m = *find_if
		(ob.buildList.begin(), ob.buildList.end(), [mapNumber](const Pair &p)
		{	return p.second == mapNumber;	}
		);

		string name = m.first;

		MapVec::iterator it = ob.buildingIDsMap.end(); --it;
		MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
		int j = 0;
		if (ob.buildingIDsMap.size() == 1)
			j = 0;
		else {
		do
		{
			string dec = it->first;
			dec.erase(dec.size() - 3, 3);

			if (dec == name /*&&it->second != -1*/)
			{
				j = stoi(it->first.substr(it->first.length() - 2)) + 1;
				break;
			}
			--it;
		} while (it != itbegin);
		if (it == itbegin)
			j = 0;
		}

		string add = "";
		if (j > 9)
		{
			add = "_";
		}
		else
		{
			add = "_0";
		}
		
		name = name + add;
		name += to_string(j);

		ob.buildingIDsMap.insert(pair<string, vector<int>>(name, { param.idNumber, param.increment }));

		if (mapNumber == 28 || mapNumber == 30 || mapNumber == 32)	//barracks_with_reactor_001
		{
			name.erase(name.size() - 1, 1);
			name += "0";
			name += to_string(j);

			ob.buildingIDsMap.insert(pair<string, vector<int>>(name, { param.idNumber, param.increment }));
		}

		++param.increment;
	}
}

// checkFinishedBuildings	: this function decreases every timer on the beingCreated matrix
//							: if a timer reaches 0, then the building is created
void Simulation::checkFinishedBuildings(Terran &ob, Parameter &param)
{
	int unsigned i = 0;
	ob.finish.clear();
	for (; i<ob.beingCreated.size(); ++i)
	{
		--ob.beingCreated[i][1];			// decrement of remainiòg time
		if (ob.beingCreated[i][1] == 0)		// remaining time is 0 -> finished!
		{
			int mapNumber = ob.beingCreated[i][0];
			ob.finish.push_back(mapNumber);

			if (mapNumber == 0)				//	SCV
			{
				++param.workersInMinerals;
			}

			if (mapNumber > 0)
			{
				Pair m = *find_if
				(ob.buildList.begin(), ob.buildList.end(), [mapNumber](const Pair &p)
				{	return p.second == mapNumber;	}
				);

				string name = m.first;

				MapVec::iterator it = ob.buildingIDsMap.begin();
				MapVec::iterator itend = ob.buildingIDsMap.end();
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					if (dec == name && it->second[1] > 0)
					{
						it->second[1] = 0;	// has to be written to JSON
						param.idNumber = it->second[0];
						break;
					}
					++it;
				} while (it != itend);
			}
			else	// SCV
			{
				MapVec::iterator it = ob.workerIDsMap.begin();
				MapVec::iterator itend = ob.workerIDsMap.end();
				do
				{
					if (it->second[1] > 0)
					{
						it->second[1] = 0;	// has to be written to JSON
						param.idNumber = it->second[0];
						break;
					}
					++it;
				} while (it != itend);
			}


			int producedby = ob.buildDetails[mapNumber][8];

			/*if (producedby == 13)*/
	
				if (mapNumber == 0)	// SCV
				{
					if (param.idNumber == 99)
					{
				string key = "init_command_center";

				MapVec::iterator it = ob.buildingIDsMap.end(); --it;
				MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
				do
				{
					string dec = it->first;

					if (dec == key)
					{
						it->second[0] = -1;
						break;
					}
					--it;
				} while (it != itbegin);				//"init_command_center"
				if (it != itbegin) goto stop;
					}

					else
					{
				string key = "orbital_command";

				MapVec::iterator it = ob.buildingIDsMap.end(); --it;
				MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					int id = -1;
					if (it->first == "init_command_center") {}
					else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits

					if (dec == key && it->second[0] == 0 && it->second[1] < 1 && param.idNumber == id)
					{
						it->second[0] = -1;
						break;
					}
					--it;
				} while (it != itbegin);			//"orbital_command"
				if (it != itbegin) goto stop;
				
				key = "command_center";
				
				it = ob.buildingIDsMap.end(); --it;
				itbegin = ob.buildingIDsMap.begin(); --itbegin;
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					if (dec == key && it->second[1] < 1)
					{
						it->second[0] = -1;
						break;
					}
					--it;
				} while (it != itbegin);			//"orbital_command"
					}

			stop:
				{}
				}
				/*else // upgraded
				{

				}*/
			//}
			else
			{
				Pair m = *find_if
				(ob.buildList.begin(), ob.buildList.end(), [producedby](const Pair &p)
				{	return p.second == producedby;	}
				);

				string name = m.first;

				if ((name == "barracks" || name == "factory" || name == "starport")
					&& ob.buildDetails[ob.buildDetails[mapNumber][8]][0] == 0
					&& (mapNumber < 10 && (mapNumber != 6 && mapNumber != 7)))
				{
					name += "_with_reactor";
					MapVec::iterator it = ob.buildingIDsMap.end(); --it;
					MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
					do
					{
						string dec = it->first;
						dec.erase(dec.size() - 3, 3);

						int id = -1;
						if (it->first != "init_command_center")
						{
							int id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
							if (dec == name && it->second[0] != -1 && id == param.idNumber)
							{
								break;
							}
						}
						--it;
					} while (it != itbegin);
					if (it == itbegin)
					{
						name += "_";
						it = ob.buildingIDsMap.end(); --it;
						itbegin = ob.buildingIDsMap.begin(); --itbegin;
						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 3, 3);

							int id = -1;
							if (it->first != "init_command_center")
							{
								int id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
								if (dec == name && it->second[0] != -1 && id == param.idNumber)
								{
									break;
								}
							}
							--it;
						} while (it != itbegin);
						if (it != itbegin)
						{
							it->second[0] = -1;		// CLEAR producedby building!!
						}
					}
					else
					{
						it->second[0] = -1;
					}
				}
				else
				{
					MapVec::iterator it = ob.buildingIDsMap.end(); --it;
					MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
					do
					{
						string dec = it->first;
						dec.erase(dec.size() - 3, 3);

						if (it->first != "init_command_center")
						{
							int id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
							if (dec == name && it->second[0] != -1 && id == param.idNumber)
							{
								break;
							}
						}
						--it;
					} while (it != itbegin);
					if (it != itbegin)
					{
						it->second[0] = -1;		// CLEAR producedby building!!
					}
				}
			}

			++ob.buildDetails[mapNumber][0];	// increment the quantity of finished item

			param.totalSupCost += ob.buildDetails[mapNumber][5];

			if (producedby == 0)				// Produced by SCV
				++param.workersInMinerals;

			if (mapNumber == 14)				// orbital_command
			{
				param.energy += ob.buildDetails[mapNumber][5];
				param.energyCollect = 1;
			}

			//------------################### to get over !!
			if (mapNumber > 27) // something with tech lab
				--ob.buildDetails[producedby][0]; // --something
			if (mapNumber == 14 || mapNumber == 15) // init_command_center upgrated
			{
				--ob.buildDetails[producedby][0]; // --command_center
			}
			//------------################### to get over !!

			ob.beingCreated[i][0] = -1;
			ob.beingCreated[i][1] = -1;

			if (mapNumber == 16)	// refinery distribution
			{
				int workers_available = 0;
				for (int unsigned j = 0; j < sizeof(ob.initWorkerIDs) / sizeof(int); ++j)
				{
					if (ob.initWorkerIDs[j] == -1)
					{
						ob.initWorkerIDs[j] = -2;	//refinery
						++workers_available;
						if (workers_available == 3)
							break;
					}
				}
				if (workers_available < 3)
				{
					for (int unsigned j = 0; j < ob.workerIDs.size(); ++j)
					{
						if (ob.workerIDs[j] == -1)
						{
							ob.workerIDs[j] = -2;	//refinery
							++workers_available;
							if (workers_available == 3)
								break;
						}
					}
				}
				param.workersInVespene = param.workersInVespene + workers_available;
				param.workersInMinerals = param.workersInMinerals - workers_available;
			}
		}
	}
}

//	checkCreated		: collects information about items which are still in process of building
//						: is used at the end of simulation to be sure that all buildings are done
//						: everything is done -> returns 1
int Simulation::checkCreated(Terran ob)
{
	// First row	= what is being created (int from map)
	// Second row	= time left

	int h = 0;
	for (int unsigned i = 0; i < ob.beingCreated.size(); ++i)
	{
		if (ob.beingCreated[i][1] > 0) h++;
	}
	if (!h) return 1; else return 0;
}

void Simulation::writeJson(string type, int mapNumber, Terran &ob, Parameter &param)
{
	ofstream json;
	json.open("stdout.json", fstream::app);

	if (param.Time > param.previousTime)
	{
		param.wasJsonWritten = 1;
		json << "{\n\"time\": " << param.Time;
		json << ",\n\"status\": { \n\"workers\": {";
		json << "\n\"minerals\": " << param.workersInMinerals;
		json << ",\n\"vespene\": " << param.workersInVespene;
		json << "\n},\n\"resources\": {\n";
		json << "\n\"minerals\": " << param.minerals;
		json << ",\n\"vespene\": " << param.vespene;
		json << ",\n\"supply-used\": " << param.supplyUsed;
		json << ",\n\"supply\": " << param.totalSupCost;
		json << "\n}\n},\n\"events\": [\n";
	}
	else
	{
		json << ",\n";
	}

	json << "{\n\"type\": \"" << type << "\",\n";

	// Lambda Function to find the name of the build

	string producerID;
	string producedIDs;

	//------------producedIDs!!
	if (mapNumber == 0)
	{
		MapVec::iterator it = ob.workerIDsMap.begin();
		MapVec::iterator itend = ob.workerIDsMap.end();
		do
		{
			if (it->second[0] >= 0)
			{
				producedIDs = it->first;
				param.idNumber = it->second[0];

				if (type == "build-end")
				{
					it->second[1] = -1;
					break;
				}
				//break;
			}
			++it;
		} while (it != itend);
	}
	else if (mapNumber > 0)
	{
		Pair v = *find_if
		(ob.buildList.begin(), ob.buildList.end(), [mapNumber](const Pair &p)
		{	return p.second == mapNumber;	}
		);
		string key = v.first;
		
		MapVec::iterator it = ob.buildingIDsMap.begin();
		MapVec::iterator itend = ob.buildingIDsMap.end();
		do
		{
			string dec = it->first;
			dec.erase(dec.size() - 3, 3);
			
			if (mapNumber >= 13)	// Building
			{
				if (dec == key)	//when it is 0 means it has not been written to JSON yet
				{
					producedIDs = it->first;
					param.idNumber = it->second[0];

					if (it->second[1] == 0)
					{
						if (type == "build-end")
						{
							it->second[0] = -1;
							it->second[1] = -1;

							if (mapNumber == 28 || mapNumber == 30 || mapNumber == 32)
							{
								string name = producedIDs;
								int id = stoi(name.substr(name.length() - 1));
								name.erase(name.size() - 1, 1);
								name += "0";
								name += to_string(id);

								it = ob.buildingIDsMap.begin();
								itend = ob.buildingIDsMap.end();
								do
								{
									string dec = it->first;

									if (dec == name)
									{
										it->second[0] = -1;
										it->second[1] = -1;
										break;
									}
									++it;
								} while (it != itend);
							}
						}
						break;
					}
				}
			}
			else	// Unit
			{
				if (dec == key /*&& it->second[1] == 0*/)	//when it is 0 means it has not been written to JSON yet
				{

					producedIDs = it->first;
					if (it->second[1] == 0)
					{
						if (type == "build-end")
						{
							it->second[1] = -1;
						}
						break;
					}
				}
			}
			++it;
		} while (it != itend);
	}
	//------------producedIDs!!

	//-------------producerID!!
	int producedby = ob.buildDetails[mapNumber][8];

	if (producedby == 0) //produced by SCV
	{
		int unsigned j = -1;
		int unsigned h = -1;
		do
		{
			++j;
			if (j == param.idNumber && ob.initWorkerIDs[j] == mapNumber)
				break;
		} while (j <= sizeof(ob.initWorkerIDs) / sizeof(int) - 1); // ÌÎÃÓÒ ÁÛÒÜ ÄÂÀ ÊÎÌÌÀÍÄÍÛÉ ÖÅÍÒÐÀ ÂÛÏÓÑÊÀÞÙÈÅ ÐÀÁÎÒÍÈÊÎÂ!!!
		if (j > sizeof(ob.initWorkerIDs) / sizeof(int) - 1)
		{
			do
			{
				++h;
			} while (ob.workerIDs[h] != mapNumber && h <= ob.workerIDs.size() - 1);
		}
		if (j <= sizeof(ob.initWorkerIDs) / sizeof(int) - 1)
		{
			Pair m = *find_if
			(ob.initWorkerIDsMap.begin(), ob.initWorkerIDsMap.end(), [j](const Pair &p)
			{	return p.second == signed(j);	}
			);
			producerID = m.first;
		}
		else
		{
			MapVec::iterator it = ob.workerIDsMap.end(); --it;
			MapVec::iterator itbegin = ob.workerIDsMap.begin(); --itbegin;
			do
			{
				int id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
				if (h == id)
				{
					producerID = it->first;
					break;
				}
				--it;
			} while (it != itbegin);
			producerID = it->first;
		}

		if (type == "build-end")
		{
			if (j < 6) ob.initWorkerIDs[j] = -1;
			else ob.workerIDs[h] = -1;
		}		//workerIDs & initWorkerIDs
	}
	else if (producedby == 13)
	{
		if (mapNumber == 14 || mapNumber == 15) // init_command_center upgrated
		{
			string key = "init_command_center";

			MapVec::iterator it = ob.buildingIDsMap.end(); --it;
			MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
			do
			{
				string dec = it->first;

				if (param.idNumber == 99 && dec == key)
				{
					producerID = key;
					if (type == "build-end")
					{
						it = ob.buildingIDsMap.erase(it);
					}
					break;
				}
				--it;
			} while (it != itbegin);
			if (it == itbegin)
			{
				string key = "command_center";	// command_center upgrated

				MapVec::iterator it = ob.buildingIDsMap.end(); --it;
				MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					if (dec == key)
					{
						producerID = it->first;
						if (type == "build-end")
						{
							it = ob.buildingIDsMap.erase(it);
						}
						break;
					}
					--it;
				} while (it != itbegin);
			}
		}
		else	// SCV
		{
			if (param.idNumber == 99)
			{
			string key = "init_command_center";

			MapVec::iterator it = ob.buildingIDsMap.end(); --it;
			MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
			do
			{
				string dec = it->first;

				if (dec == key)
				{
					producerID = key;
					if (type == "build-end")
						ob.workerIDsMap.find(producedIDs)->second[0] = -1;
					break;
				}
				--it;
			} while (it != itbegin);				//"init_command_center"
			}

			string key = "orbital_command";

			MapVec::iterator it = ob.buildingIDsMap.end(); --it;
			MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
			do
			{
				string dec = it->first;
				dec.erase(dec.size() - 3, 3);

				int id = -1;
				if (it->first == "init_command_center") {}
				else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
				
				if (dec == key && it->second[1] == -1 && id == ob.workerIDsMap.find(producedIDs)->second[0])
				{
					producerID = it->first;
					if (type == "build-end")
						ob.workerIDsMap.find(producedIDs)->second[0] = -1;
					break;
				}
				--it;
			} while (it != itbegin);				//"orbital_command"
			if (it != itbegin) goto stop;

			key = "command_center";

			it = ob.buildingIDsMap.end(); --it;
			itbegin = ob.buildingIDsMap.begin(); --itbegin;
			do
			{
				string dec = it->first;
				dec.erase(dec.size() - 3, 3);

				int id = -1;
				if (it->first == "init_command_center") {}
				else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits


				if (dec == key && it->second[1] == -1 && id == ob.workerIDsMap.find(producedIDs)->second[0])
				{
					producerID = it->first;
					if (type == "build-end")
						ob.workerIDsMap.find(producedIDs)->second[0] = -1;
					break;
				}
				--it;
			} while (it != itbegin);
			if (it != itbegin) goto stop;

			key = "planetary_fortress";

			it = ob.buildingIDsMap.end(); --it;
			itbegin = ob.buildingIDsMap.begin(); --itbegin;
			do
			{
				string dec = it->first;
				dec.erase(dec.size() - 3, 3);

				int id = -1;
				if (it->first == "init_command_center") {}
				else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits

				if (dec == key && it->second[1] == -1 && id == ob.workerIDsMap.find(producedIDs)->second[0])
				{
					producerID = it->first;
					if (type == "build-end")
						ob.workerIDsMap.find(producedIDs)->second[0] = -1;
					break;
				}
				--it;
			} while (it != itbegin);

			stop:
				{}
		}
	}
	else
	{
		Pair m = *find_if
		(ob.buildList.begin(), ob.buildList.end(), [producedby](const Pair &p)
		{	return p.second == producedby;	}
		);
		
		producerID = m.first;

		if ((producerID == "barracks" || producerID == "factory" || producerID == "starport")
			&& ob.buildDetails[ob.buildDetails[mapNumber][8]][0] == 0
			&& (mapNumber < 10 && (mapNumber != 6 && mapNumber != 7)))
		{
			producerID += "_with_reactor";
			MapVec::iterator it = ob.buildingIDsMap.end(); --it;
			MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
			do
			{
				string dec = it->first;
				dec.erase(dec.size() - 3, 3);

				int id = -1;
				if (it->first == "init_command_center") {}
				else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
				if (dec == producerID && it->second[1] == -1 && id == ob.buildingIDsMap.find(producedIDs)->second[0])
				{
					producerID = it->first;
					if (type == "build-end" && (mapNumber > 27)) // something with tech_lab
						it = ob.buildingIDsMap.erase(it);
					break;
				}
				--it;
			} while (it != itbegin);
			if (it == itbegin)
			{
				producerID += "_";
				it = ob.buildingIDsMap.end(); --it;
				itbegin = ob.buildingIDsMap.begin(); --itbegin;
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					int id = -1;
					if (it->first == "init_command_center") {}
					else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
					if (dec == producerID && it->second[1] == -1 && id == ob.buildingIDsMap.find(producedIDs)->second[0])
					{
						producerID = it->first;
						if (type == "build-end" && (mapNumber > 27)) // something with tech_lab
							it = ob.buildingIDsMap.erase(it);
						break;
					}
					--it;
				} while (it != itbegin);
			}
		}
		else
		{
			MapVec::iterator it = ob.buildingIDsMap.end(); --it;
			MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
			do
			{
				string dec = it->first;
				dec.erase(dec.size() - 3, 3);

				int id = -1;
				if (it->first == "init_command_center") {}
				else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
				if (dec == producerID && it->second[1] == -1 && id == ob.buildingIDsMap.find(producedIDs)->second[0])
				{
					producerID = it->first;
					if (type == "build-end" && (mapNumber > 27)) // something with tech_lab
						it = ob.buildingIDsMap.erase(it);
					break;
				}
				--it;
			} while (it != itbegin);
		}
	}
	//-------------producerID!!

	Pair name = *find_if
	(ob.buildList.begin(), ob.buildList.end(), [mapNumber](const Pair &p)
	{	return p.second == mapNumber;	}
	);

	if (type == "build-start")
		json << "\"name\": \"" << name.first << "\",\n\"producerID\": \"" << producerID << "\"\n}";
	else
		json << "\"name\": \"" << name.first << "\",\n\"producerID\": \"" << producerID << "\",\n\"producedIDs\": [\"" << producedIDs << "\"]\n}";

	param.previousTime = param.Time;
	json.close();
}

void Simulation::run(Simulation *sim, Terran *terran, Parameter &param, string &actualEvent, ifstream &fin)
{
	sim->checkFinishedBuildings(*terran, param);

	if (end == 0)
	{
		buildSuccess = sim->createCurrentBuild(actualEvent, *terran, param);
		if (terran->finish.size() > 0)
			for (vector<int>::iterator it = terran->finish.begin(); it != terran->finish.end(); ++it)
				sim->writeJson("build-end", *it, *terran, param);
		if (terran->start.size() > 0)
			for (int i = buildSuccess; i != 0; i = sim->createCurrentBuild(actualEvent, *terran, param))
			{
				sim->writeJson("build-start", terran->buildList[actualEvent], *terran, param);
				fin >> actualEvent;
				if (fin.eof()) { end = 1; }
			}
	}
	else
	{
		if (!over)		buildSuccess = sim->createCurrentBuild(actualEvent, *terran, param);

		if (terran->finish.size() > 0)
			for (vector<int>::iterator it = terran->finish.begin(); it != terran->finish.end(); ++it)
				sim->writeJson("build-end", *it, *terran, param);
		if (terran->start.size() > 0)
			for (int i = buildSuccess; i != 0; i = sim->createCurrentBuild(actualEvent, *terran, param))
			{
				sim->writeJson("build-start", terran->buildList[actualEvent], *terran, param);
				over = 1;							// means we started building the last item of buildlist
			}
		if (terran->finish.size() > 0)
		{
			if (sim->checkCreated(*terran) == 1 && over == 1)
			{
				param.wasJsonWritten = 2;
				error = 1;		// We've done!
			}
		}
	}
}