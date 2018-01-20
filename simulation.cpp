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
	if (mapNumber > 0 && mapNumber < 13 /* && ob.beingCreated.size() > 0*/&& producedby != 20 && producedby != 21 && producedby != 25)
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

			if (dec == producedbyName && it->second[0] < 1/*it->second[0] == -1*/)
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

	if ((ob.buildDetails[producedby][0] == 0 && producedby != 0) && producedby != 13 && producedby != 20 && producedby != 21 && producedby != 25)
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
			} while (ob.initWorkerIDs[j][0] != -1 && (j <= sizeof(ob.initWorkerIDs) / sizeof(int) - 1));
			if (j < 6)
			{
				ob.initWorkerIDs[j][0] = mapNumber;
				param.idNumber = j;
				param.idInitWorker = 1;
			}
			else
			{
				int unsigned h = -1;
				do
				{
					++h;
				} while (ob.workerIDs[h][0] != -1 && (h <= ob.workerIDs.size() - 1));
				if (h == ob.workerIDs.size() - 1)
				{
					return 0;	//no worker available 
				}
				else
				{
					ob.workerIDs[h][0] = mapNumber;
					param.idNumber = h;
				}
			}
			--param.workersInMinerals;
		}
		else if (producedby == 13 && mapNumber == 0)	// SCV
		{
			string key = "init_command_center";

			MapVec::iterator it = ob.buildingIDsMap.begin();
			MapVec::iterator itend = ob.buildingIDsMap.end();
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

				MapVec::iterator it = ob.buildingIDsMap.begin();
				MapVec::iterator itend = ob.buildingIDsMap.end();
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					if (dec == key && it->second[0] == -1 && it->second[1] < 1)
					{
						break;
					}
					++it;
				} while (it != itend);
				if (it != itend)
				{
					it->second[0] = mapNumber;
					param.idNumber = stoi(it->first.substr(it->first.length() - 2));
				}
				else
				{
					string key = "command_center";

					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
					do
					{
						string dec = it->first;
						dec.erase(dec.size() - 3, 3);

						if (dec == key && it->second[0] == -1 && it->second[1] < 1)
						{
							break;
						}
						++it;
					} while (it != itend);
					if (it != itend)
					{
						it->second[0] = mapNumber;
						param.idNumber = stoi(it->first.substr(it->first.length() - 2));
					}
					else
					{
						string key = "planetary_fortress";

						MapVec::iterator it = ob.buildingIDsMap.begin();
						MapVec::iterator itend = ob.buildingIDsMap.end();
						do
						{
							string dec = it->first;
							dec.erase(dec.size() - 3, 3);

							if (dec == key && it->second[0] == -1 && it->second[1] < 1)
							{
								break;
							}
							++it;
						} while (it != itend);
						if (it != itend)
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

			MapVec::iterator it = ob.buildingIDsMap.begin();
			MapVec::iterator itend = ob.buildingIDsMap.end();
			do
			{
				string dec = it->first;

				if (dec == key && it->second[0] == -1)
				{
					break;
				}
				++it;
			} while (it != itend);
			if (it != itend)
			{
				it->second[0] = mapNumber;
				param.idNumber = 99;
			}
			else
			{
				string key = "command_center";

				MapVec::iterator it = ob.buildingIDsMap.begin();
				MapVec::iterator itend = ob.buildingIDsMap.end();
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					if (dec == key && it->second[0] == -1)
					{
						break;
					}
					++it;
				} while (it != itend);
				if (it != itend)
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

			MapVec::iterator it = ob.buildingIDsMap.begin();
			MapVec::iterator itend = ob.buildingIDsMap.end();
			do
			{
				string dec = it->first;
				dec.erase(dec.size() - 3, 3);

				if (dec == name && (it->second[0] == -1 || it->second[0] == -2) && it->second[1] < 1)
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
				++it;
			} while (it != itend);
			if (it == itend)
			{
				if (producedby != 20 && producedby != 21 && producedby != 25)
					return 0;
				else
				{
					if (producedby == 20)
					{
						string name = "barracks_with_reactor";

						MapVec::iterator it = ob.buildingIDsMap.begin();
						MapVec::iterator itend = ob.buildingIDsMap.end();
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
							++it;
						} while (it != itend);
						if (it != itend)
							goto stop;

						name = "barracks_with_reactor_00";

						it = ob.buildingIDsMap.begin();
						itend = ob.buildingIDsMap.end();

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
							++it;
						} while (it != itend);
						if (it != itend)
							goto stop;
						else
							return 0;

					stop:
						{}
					}
					else if (producedby == 21)
					{
						string name = "factory_with_reactor";

						MapVec::iterator it = ob.buildingIDsMap.begin();
						MapVec::iterator itend = ob.buildingIDsMap.end();
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
							++it;
						} while (it != itend);
						if (it != itend)
							goto stop2;

						name = "factory_with_reactor_00";

						it = ob.buildingIDsMap.begin();
						itend = ob.buildingIDsMap.end();

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
							++it;
						} while (it != itend);
						if (it != itend)
							goto stop2;
						else
							return 0;

					stop2:
						{}
					}
					else if (producedby == 25)
					{
						string name = "starport_with_reactor";

						MapVec::iterator it = ob.buildingIDsMap.begin();
						MapVec::iterator itend = ob.buildingIDsMap.end();
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
							++it;
						} while (it != itend);
						if (it != itend)
							goto stop3;

						name = "starport_with_reactor_00";

						it = ob.buildingIDsMap.begin();
						itend = ob.buildingIDsMap.end();

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
							++it;
						} while (it != itend);
						if (it != itend)
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

		int producedby = ob.buildDetails[mapNumber][8];

		if (producedby != 0) ob.buildingIDsMap.insert(pair<string, vector<int>>(name, { param.idNumber, param.increment }));
		else
		{
			ob.buildingIDsMap.insert(pair<string, vector<int>>(name, { -2, param.increment }));
			if (param.idInitWorker > 0)
			{
				ob.initWorkerIDs[param.idNumber][1] = stoi(name.substr(name.length() - 2));
				param.idInitWorker = 0;
			}
			else ob.workerIDs[param.idNumber][1] = stoi(name.substr(name.length() - 2));
		}

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

						if (mapNumber == 14)				// orbital_command
						{
							ob.specialbeingCreated.push_back(vector<double>(2, -1));

							if (param.energyCollect)
							{
								int id = stoi(it->first.substr(it->first.length() - 1));

								ob.specialbeingCreated[id][0] = 50;
							}
							else
								ob.specialbeingCreated[0][0] = 50;

							param.energyCollect = 1;
						}
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

					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
					do
					{
						string dec = it->first;

						if (dec == key)
						{
							it->second[0] = -1;
							break;
						}
						++it;
					} while (it != itend);
					if (it != itend) goto stop;
				}
				else
				{
					string key = "orbital_command";

					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
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
						++it;
					} while (it != itend);
					if (it != itend) goto stop;
				
					key = "command_center";
				
					it = ob.buildingIDsMap.begin();
					itend = ob.buildingIDsMap.end();
					do
					{
						string dec = it->first;
						dec.erase(dec.size() - 3, 3);

						if (dec == key && it->second[1] < 1)
						{
							it->second[0] = -1;
							break;
						}
						++it;
					} while (it != itend);
					if (it != itend) goto stop;

					key = "planetary_fortress";

					it = ob.buildingIDsMap.begin();
					itend = ob.buildingIDsMap.end();
					do
					{
						string dec = it->first;
						dec.erase(dec.size() - 3, 3);

						if (dec == key && it->second[1] < 1)
						{
							it->second[0] = -1;
							break;
						}
						++it;
					} while (it != itend);
				}
			stop:
				{}
			}
			else
			{
				Pair m = *find_if
				(ob.buildList.begin(), ob.buildList.end(), [producedby](const Pair &p)
				{	return p.second == producedby;	}
				);

				string name = m.first;

				if ((name == "barracks" || name == "factory" || name == "starport")
					//&& ob.buildDetails[ob.buildDetails[mapNumber][8]][0] == 0
					&& (mapNumber < 10 && (mapNumber != 6 && mapNumber != 7)))
				{
					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
					do
					{
						string dec = it->first;
						dec.erase(dec.size() - 3, 3);

						int id = -1;
						if (it->first == "init_command_center") {}
						else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
						if (dec == name && it->second[0] != -1 && id == param.idNumber)
						{
							break;
						}
						++it;
					} while (it != itend);
					if (it == itend)
					{
						name += "_with_reactor";
						it = ob.buildingIDsMap.begin();
						itend = ob.buildingIDsMap.end();
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
							++it;
						} while (it != itend);
						if (it == itend)
						{
							name += "_";
							it = ob.buildingIDsMap.begin();
							itend = ob.buildingIDsMap.end();
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
								++it;
							} while (it != itend);
							if (it != itend)
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
						it->second[0] = -1;
					}
				}
				else if (name == "barracks" || name == "factory" || name == "starport")
				{
					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
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
						++it;
					} while (it != itend);
					if (it != itend)
					{
						it->second[0] = -1;		// CLEAR producedby building!!
					}
				}
				else
				{
					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
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
						++it;
					} while (it != itend);
					if (it != itend)
					{
						it->second[0] = -1;		// CLEAR producedby building!!
					}
				}
			}

			++ob.buildDetails[mapNumber][0];	// increment the quantity of finished item

			param.totalSupCost += ob.buildDetails[mapNumber][5];

			if (producedby == 0)				// Produced by SCV
				++param.workersInMinerals;

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
					if (ob.initWorkerIDs[j][0] == -1)
					{
						ob.initWorkerIDs[j][0] = -2;	//refinery
						++workers_available;
						if (workers_available == 3)
							break;
					}
				}
				if (workers_available < 3)
				{
					for (int unsigned j = 0; j < ob.workerIDs.size(); ++j)
					{
						if (ob.workerIDs[j][0] == -1)
						{
							ob.workerIDs[j][0] = -2;	//refinery
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

	if (param.energyCollect)
	{
		i = 0;
		for (; i < ob.specialbeingCreated.size(); ++i)
		{
			--ob.specialbeingCreated[i][1];			// decrement of remainiòg time

			if (ob.specialbeingCreated[i][0] < 200) ob.specialbeingCreated[i][0] += 0.5625;
			else ob.specialbeingCreated[i][0] = 200;

			if (ob.specialbeingCreated[i][1] == 0)		// remaining time is 0 -> finished!
			{
				ob.specialbeingCreated[i][1] = -1;

				param.mule -= 4;
			}
		}
	}
}

int Simulation::specialEvent(Terran &ob, Parameter &param)
{
	if (param.energyCollect)
	{
		int i = 0;
		do
		{
			if (ob.specialbeingCreated[i][1] < 0 && ob.specialbeingCreated[i][0] >= 50)
			{
				ob.specialbeingCreated[i][0] -= 50;
				ob.specialbeingCreated[i][1] = 90;

				param.mule += 4;

				return 1;
			}
			++i;
		} while (i < ob.specialbeingCreated.size());
	}

	return 0;
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
	
	if (type == "build-start" || type == "build-end")
	{
		string producerID;
		string producedIDs;

		//------------producedIDs!!
		if (mapNumber == 0)
		{
			MapVec::iterator it = ob.workerIDsMap.begin();
			MapVec::iterator itend = ob.workerIDsMap.end();
			do
			{
				if (it->second[1] >= 0)
				{
					producedIDs = it->first;
					param.idNumber = it->second[0];

					if (type == "build-end")
					{
						it->second[1] = -1;
						break;
					}
				}
				++it;
			} while (it != itend);
		}
		else if (ob.buildDetails[mapNumber][8] == 0)	// produced by SCV
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
						param.idNumber = stoi(it->first.substr(it->first.length() - 2));;

						if (it->second[1] == 0)
						{
							if (type == "build-end")
							{
								it->second[0] = -1;
								it->second[1] = -1;
							}
							break;
						}
					}
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
											//it->second[0] = -1;
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
				if (ob.initWorkerIDs[j][1] == param.idNumber && ob.initWorkerIDs[j][0] == mapNumber)
					break;
			} while (j <= sizeof(ob.initWorkerIDs) / sizeof(int) - 1);
			if (j > sizeof(ob.initWorkerIDs) / sizeof(int) - 1)
			{
				do
				{
					++h;
					if (ob.initWorkerIDs[j][1] == param.idNumber && ob.initWorkerIDs[j][0] == mapNumber)
						break;
				} while (h <= ob.workerIDs.size() - 1);
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

			if (type == "build-end") //workerIDs & initWorkerIDs
			{
				if (j < 6)
				{
					ob.initWorkerIDs[j][0] = -1;
					ob.initWorkerIDs[j][1] = -1;
				}
				else
				{
					ob.workerIDs[h][0] = -1;
					ob.workerIDs[h][1] = -1;
				}
			}		
		}
		else if (producedby == 13)
		{
			if (mapNumber == 14 || mapNumber == 15) // init_command_center upgrated
			{
				string key = "init_command_center";

				MapVec::iterator it = ob.buildingIDsMap.begin();
				MapVec::iterator itend = ob.buildingIDsMap.end();
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
					++it;
				} while (it != itend);
				if (it == itend)
				{
					string key = "command_center";	// command_center upgrated

					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
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
						++it;
					} while (it != itend);
				}
			}
			else	// SCV
			{
				if (param.idNumber == 99)
				{
					string key = "init_command_center";

					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
					do
					{
						string dec = it->first;

						if (dec == key)
						{
							producerID = key;
							if (type == "build-end")
							{
								ob.workerIDsMap.find(producedIDs)->second[0] = -1;
							}
							break;
						}
						++it;
					} while (it != itend);
				}
				else
				{
					string key = "orbital_command";

					MapVec::iterator it = ob.buildingIDsMap.begin();
					MapVec::iterator itend = ob.buildingIDsMap.end();
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
							{
								ob.workerIDsMap.find(producedIDs)->second[0] = -1;
								ob.buildingIDsMap.find(producerID)->second[0] = -1;
							}
							break;
						}
						++it;
					} while (it != itend);
					if (it != itend) goto stop;

					key = "command_center";

					it = ob.buildingIDsMap.begin();
					itend = ob.buildingIDsMap.end();
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
							{
								ob.workerIDsMap.find(producedIDs)->second[0] = -1;
								ob.buildingIDsMap.find(producerID)->second[0] = -1;
							}
							break;
						}
						++it;
					} while (it != itend);
					if (it != itend) goto stop;

					key = "planetary_fortress";

					it = ob.buildingIDsMap.begin();
					itend = ob.buildingIDsMap.end();
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
							{
								ob.workerIDsMap.find(producedIDs)->second[0] = -1;
								ob.buildingIDsMap.find(producerID)->second[0] = -1;
							}
							break;
						}
						++it;
					} while (it != itend);

				stop:
					{}
				}
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
				//&& ob.buildDetails[ob.buildDetails[mapNumber][8]][0] == 0
				&& (mapNumber < 10 && (mapNumber != 6 && mapNumber != 7)))
			{
				MapVec::iterator it = ob.buildingIDsMap.begin();
				MapVec::iterator itend = ob.buildingIDsMap.end();
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
						break;
					}
					++it;
				} while (it != itend);
				if (it == itend)
				{
					producerID += "_with_reactor";

					it = ob.buildingIDsMap.begin();
					itend = ob.buildingIDsMap.end();
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
							/*if (type == "build-end" && (mapNumber > 27)) // something with tech_lab
								it = ob.buildingIDsMap.erase(it);*/
							break;
						}
						++it;
					} while (it != itend);

					if (it == itend)
					{
						producerID += "_";
						it = ob.buildingIDsMap.begin();
						itend = ob.buildingIDsMap.end();
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
								/*if (type == "build-end" && (mapNumber > 27)) // something with tech_lab
									it = ob.buildingIDsMap.erase(it);*/
								break;
							}
							++it;
						} while (it != itend);
					}
				}
			}
			else if (producerID == "barracks" || producerID == "factory" || producerID == "starport")
			{
				MapVec::iterator it = ob.buildingIDsMap.begin();
				MapVec::iterator itend = ob.buildingIDsMap.end();

				string producedIDs_000 = producedIDs + "0";
				do
				{
					string dec = it->first;
					dec.erase(dec.size() - 3, 3);

					int id = -1;
					if (it->first == "init_command_center") {}
					else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits
					if (dec == producerID && it->second[1] == -1 && id == ob.buildingIDsMap.find(producedIDs_000)->second[0])
					{
						producerID = it->first;
						if (type == "build-end" && (mapNumber > 27)) // something with tech_lab
						{
							it = ob.buildingIDsMap.erase(it);
							ob.buildingIDsMap.find(producedIDs_000)->second[0] = -1;
						}
						break;
					}
					++it;
				} while (it != itend);
			}
			else
			{
				MapVec::iterator it = ob.buildingIDsMap.begin();
				MapVec::iterator itend = ob.buildingIDsMap.end();
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
						//if (type == "build-end" && (mapNumber > 27)) // something with tech_lab
						//	it = ob.buildingIDsMap.erase(it);
						break;
					}
					++it;
				} while (it != itend);
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
	}
	else	// special event
	{
			int i = 0;
			for (; i < ob.specialbeingCreated.size(); ++i)
			{
				if (int(ob.specialbeingCreated[i][1]) == 90)
				{
					json << "\"name\": \"mule\",\n\"triggeredBy\" : \"orbital_command_0" << i << "\"\n}";
					break;
				}
			}
	}

	param.previousTime = param.Time;
	json.close();
}

void Simulation::run(Simulation *sim, Terran *terran, Parameter &param, string &actualEvent, ifstream &fin)
{
	sim->checkFinishedBuildings(*terran, param);

	if (end == 0)
	{
		buildSuccess = sim->createCurrentBuild(actualEvent, *terran, param);
		if (buildSuccess == 0) specialSuccess = sim->specialEvent(*terran, param);
		if (terran->finish.size() > 0)
			for (vector<int>::iterator it = terran->finish.begin(); it != terran->finish.end(); ++it)
				sim->writeJson("build-end", *it, *terran, param);
		if (specialSuccess > 0)
		{
			sim->writeJson("special", 0, *terran, param);
			specialSuccess = 0;
		}

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
		/*if (!over)*/		//buildSuccess = sim->createCurrentBuild(actualEvent, *terran, param);

		if (buildSuccess == 0) specialSuccess = sim->specialEvent(*terran, param);
		if (specialSuccess > 0)
		{
			sim->writeJson("special", 0, *terran, param);
			specialSuccess = 0;
		}
			
		if (terran->finish.size() > 0)
			for (vector<int>::iterator it = terran->finish.begin(); it != terran->finish.end(); ++it)
				sim->writeJson("build-end", *it, *terran, param);
		//if (terran->start.size() > 0)
		//	for (int i = buildSuccess; i != 0; i = sim->createCurrentBuild(actualEvent, *terran, param))
		//	{
		//		sim->writeJson("build-start", terran->buildList[actualEvent], *terran, param);
		//		over = 1;							// means we started building the last item of buildlist
		//	}
		if (terran->finish.size() > 0)
		{
			if (sim->checkCreated(*terran) == 1 /*&& over == 1*/)
			{
				param.wasJsonWritten = 2;
				error = 1;		// We've done!
			}
		}
	}
}