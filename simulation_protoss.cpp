#include "simulation_protoss.h"

//	createCurrentBuild	: building is NOT possible	-> returns 0
//						: building is possible		-> returns 1
int Simulation_P::createCurrentBuild(string actualEvent, Protoss &ob, Parameter &param)
{
    if (ob.start.size() > 0)
    {
        //only one unit is possible to start to build at one timestep
        return 0;
    }


   /* if (param.totalSupCost - param.supplyUsed == 0)
    {
        //cout<<"supply"<<endl;
        return 0;
    }
*/
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


    else if (ob.buildDetails[dependent][0] == 0) {/*cout<<"dependency"<<endl;*/ return 0; }

    if (ob.buildDetails[mapNumber][1] > param.minerals || ob.buildDetails[mapNumber][2] > param.vespene)
    {
        //cout<<"resource"<<endl;
        return 0;
    }

    if (mapNumber == 25)	// refinery limitation
    {
        string key = "assimilator";

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
    //for units //todo what is 16 here, in terran buildings start ID is 13
 /*   if (mapNumber < 14 && ob.beingCreated.size() > 0)
    {
        int unsigned b = 0;
        int check = ob.beingCreated[b][0];
        while (b != ob.beingCreated.size())
        {
            if (producedby == ob.buildDetails[check][8]) { // if satisfies -> occupied
                //cout<<"occupied"<<endl;
                return 0;
            }
            ++b;
        }		//occupied
    }*/

    if (mapNumber > 0 && mapNumber < 14) {
        Pair m = *find_if
                (ob.buildList.begin(), ob.buildList.end(),
                 [producedby](const Pair &p) { return p.second == producedby; }
                );

        string producedbyName = m.first;

        MapVec::iterator it = ob.buildingIDsMap.begin();
        MapVec::iterator itend = ob.buildingIDsMap.end();
        do {
            string dec = it->first;
            dec.erase(dec.size() - 3, 3);

            if (it->first == "init_nexus" && ob.buildingIDsMap.find(it->first)->second[0] == -1) {
                break;
            }


            if (dec == producedbyName && it->second[0] != -1) {
                break;
            }
            ++it;
        } while (it != itend);
        if (it == itend) {
            return 0;
        }
    }

    //TODO warum 14 // 13 for terran ??? upgrade maybe
    if ((ob.buildDetails[producedby][0] == 0) && producedby != 14)
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
            //TODO this might be again upgrade
        else if (producedby == 14 && (mapNumber == 0 || mapNumber == 10))
        {
            string key = "init_nexus";

            MapVec::iterator itend = ob.buildingIDsMap.end(); //--it;
            MapVec::iterator it = ob.buildingIDsMap.begin(); //--itbegin;
            do
            {
                string dec = it->first;

                if (dec == key && it->second[0]==-1)
                {
                    break;
                }
                ++it;
            } while (it != itend);				//"init_command_center"
            if (it != itend)
            {
                it->second[0] = mapNumber;
                param.idNumber=99;
            }
            else{
                key = "nexus";

                itend = ob.buildingIDsMap.end(); //--it;
                it = ob.buildingIDsMap.begin(); //--itbegin;
                do
                {
                    string dec = it->first;
                    dec.erase(dec.size()-3, 30);

                    if (dec == key && it->second[0]==-1 && it->second[1] < 1)
                    {
                        break;
                    }
                    ++it;
                } while (it != itend);				//"init_command_center"
                if (it != itend)
                {
                    it->second[0] = mapNumber;
                    param.idNumber = stoi(it->first.substr(it->first.length()-2));
                } else return 0;
            }

        }
        else {
            Pair m = *find_if
                    (ob.buildList.begin(), ob.buildList.end(),
                     [producedby](const Pair &p) { return p.second == producedby; }
                    );

            string name = m.first;

            MapVec::iterator it = ob.buildingIDsMap.end();
            --it;
            MapVec::iterator itbegin = ob.buildingIDsMap.begin();
            --itbegin;
            do {
                string dec = it->first;
                dec.erase(dec.size() - 3, 3);

                if (dec == name && it->second[0] == -1 && it->second[1]<1) {
                    it->second[0] = mapNumber;
                    param.idNumber = stoi(it->first.substr(it->first.length()-2));
                    break;
                }
                --it;
            } while (it != itbegin);
            if (it == itbegin) {
                return 0;
            }
        }

            //------------################### assimulator!!

        addBeingCreated(mapNumber, ob, param);

        return 1;
    }
}

//	addBeingCreated		: called in "createCurrentBuild" function when building is possible
void Simulation_P::addBeingCreated(int mapNumber, Protoss &ob, Parameter &param)
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

    if (mapNumber == 0)				//	probe
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
        string str ="";
        if(j>9){
             str = "probe_";
        } else{
             str = "probe_0";
        }

        str += to_string(j);
        ob.workerIDsMap.insert(pair<string, vector<int>>(str, { param.idNumber, param.increment }));
    }
    else {
        Pair m = *find_if
                (ob.buildList.begin(), ob.buildList.end(), [mapNumber](const Pair &p) { return p.second == mapNumber; }
                );

        MapVec::iterator it = ob.buildingIDsMap.end(); --it;
        MapVec::iterator itbegin = ob.buildingIDsMap.begin();--itbegin;
        int j = 0;
        if (ob.buildingIDsMap.size()<=1){

        } else{
            do
            {
                if (it != itbegin){
                    string dec = it->first;
                    dec.erase(dec.size() - 3, 3);

                    if (dec == m.first /*&&it->second != -1*/)
                    {
                        j = stoi(it->first.substr(it->first.length()-2));
                        ++j;
                        break;
                    }
                    --it;
                }

            } while (it != itbegin);
        }


        string name;
        string add = "";
        if(j>9){
             add = "_";
        } else{
             add = "_0";
        }
        name = m.first;
        name = name + add;
        name += to_string(j);

        ob.buildingIDsMap.insert(pair<string, vector<int>>(name, { param.idNumber, param.increment }));
        ++param.increment;
    }

}

// checkFinishedBuildings	: this function decreases every timer on the beingCreated matrix
//							: if a timer reaches 0, then the building is created
void Simulation_P::checkFinishedBuildings(Protoss &ob, Parameter &param)
{
    int unsigned i = 0;
    ob.finish.clear();
    for (; i<ob.beingCreated.size(); ++i)
    {
        --ob.beingCreated[i][1];			// decrement of remainig time
        if (ob.beingCreated[i][1] == 0)		// remaining time is 0 -> finished!
        {

            int mapNumber = ob.beingCreated[i][0];
            ob.finish.push_back(mapNumber);

            if (mapNumber == 25)	// assimulator distribution
            {
                int workers_available = 0;
                for (int unsigned j = 0; j < sizeof(ob.initWorkerIDs) / sizeof(int); ++j)
                {
                    // -1 indicates of workers on minerals
                    if (ob.initWorkerIDs[j] == -1)
                    {
                        // workers on vespene gas
                        ob.initWorkerIDs[j] = -2;	//assimulator
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
                            ob.workerIDs[j] = -2;	//assimulator
                            ++workers_available;
                            if (workers_available == 3)
                                break;
                        }
                    }
                }
                param.workersInVespene = param.workersInVespene + workers_available;
                param.workersInMinerals = param.workersInMinerals - workers_available; //exceptions!!
            }

            if (mapNumber == 0)				//	probe
            {
                ++param.workersInMinerals;

            }
            if (mapNumber > 0){
                Pair m = *find_if
                        (ob.buildList.begin(), ob.buildList.end(), [mapNumber](const Pair &p)
                         { return p.second == mapNumber; }
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
                        it->second[1] = 0; // has to be written to JSON
                        param.idNumber = it->second[0];
                        break;
                    }
                    ++it;
                } while (it != itend);
            } else{
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

            if (producedby == 14)
            {

                    if (param.idNumber == 99)
                    {
                        string key = "init_nexus";

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
                        } while (it != itend);				//"init_command_center"
                        if (it != itend) goto stop;
                    }

                    else
                    {
                        string key = "nexus";

                        MapVec::iterator it = ob.buildingIDsMap.end(); --it;
                        MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
                        do
                        {
                            string dec = it->first;
                            dec.erase(dec.size() - 3, 3);

                            int id = -1;
                            if (it->first == "init_nexus") {}
                            else id = stoi(it->first.substr(it->first.length() - 2)); // producerID's last 2 digits

                            if (dec == key && it->second[0] == 0 && it->second[1] < 1 && param.idNumber == id)
                            {
                                it->second[0] = -1;
                                break;
                            }
                            --it;
                        } while (it != itbegin);


                    }
                    stop:
                    {}
            }
            else
            {
                Pair m = *find_if
                        (ob.buildList.begin(), ob.buildList.end(), [producedby](const Pair &p)
                         { return p.second == producedby; }
                        );

                string name = m.first;

                MapVec::iterator it = ob.buildingIDsMap.end(); --it;
                MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
                do
                {
                    string dec = it->first;
                    dec.erase(dec.size() - 3, 3);

                    if (it->first != "init_nexus")
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
                    it->second[0] = -1; // CLEAR producedby building!!
                }
            }

            ++ob.buildDetails[mapNumber][0];	// increment the quantity of finished item

            param.totalSupCost += ob.buildDetails[mapNumber][5];

            if (producedby == 0)				// Produced by probe
                ++param.workersInMinerals;

            if (mapNumber == 14)				// Nexus
            {
                param.energy += ob.buildDetails[mapNumber][5];
                param.energyCollect = 1;
            }

            //------------################### to get over !!
            /*
             * TODO this code for update and morphing which i do not have in my race
             * if (mapNumber >= 28 && mapNumber <= 33) // barrack_with_smth 28...33
                --ob.buildDetails[producedby][0]; // barrack--
            if (mapNumber == 14 || mapNumber == 15) // init_command_center upgrated
            {
                --ob.buildDetails[producedby][0]; // command_center--
            }*/
            //------------################### to get over !!

            ob.beingCreated[i][0] = -1;
            ob.beingCreated[i][1] = -1;
            //				return 0;

        }
    }
    //		return -1;


}

//	checkCreated		: collects information about items which are still in process of building
//						: is used at the end of simulation to be sure that all buildings are done
//						: everything is done -> returns 1
int Simulation_P::checkCreated(Protoss ob)
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

void Simulation_P::writeJson(string type, int mapNumber, Protoss &ob, Parameter &param)
{
    ofstream json;
    json.open("stdout.json", fstream::app);

    if (param.Time > param.previousTime)
    {
        param.wasJsonWritten = 1;
        json << "{\n \"time\": " << param.Time;
        json << ",\n \"status\": { \n\"workers\": {";
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
                //break;
            }
            ++it;
        } while (it != itend);
    }
    else if (mapNumber > 0)
    {
        Pair v = *find_if
                (ob.buildList.begin(), ob.buildList.end(), [mapNumber](const Pair &p)
                 { return p.second == mapNumber; }
                );
        string key = v.first;

        MapVec::iterator it = ob.buildingIDsMap.begin();
        MapVec::iterator itend = ob.buildingIDsMap.end();
        do
        {
            string dec = it->first;
            dec.erase(dec.size() - 3, 3);

            if (mapNumber >= 14) // Building
            {
                if (dec == key) //when it is 0 means it has not been written to JSON yet
                {
                    producedIDs = it->first;
                    param.idNumber = it->second[0];

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
            else // Unit
            {
                if (dec == key /*&& it->second[1] == 0*/) //when it is 0 means it has not been written to JSON yet
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
    //}
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
        } while (j <= sizeof(ob.initWorkerIDs) / sizeof(int) - 1);
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
    else if (producedby == 14) {

        if (param.idNumber == 99)
        {
            string key = "init_nexus";

            MapVec::iterator it = ob.buildingIDsMap.begin();
            MapVec::iterator itend = ob.buildingIDsMap.end();
            do
            {
                string dec = it->first;

                if (dec == key)
                {
                    producerID = key;
                    if (type == "build-end")
                        if(mapNumber == 0){
                            ob.workerIDsMap.find(producedIDs)->second[0] = -1;
                        } else{
                            ob.buildingIDsMap.find(producedIDs)->second[0] = -1;
                        }
                    break;
                }
                ++it;
            } while (it != itend);				//"init_command_center"
        }
        else{
            string key = "nexus";

            MapVec::iterator it = ob.buildingIDsMap.end(); --it;
            MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
            do
            {
                string dec = it->first;
                dec.erase(dec.size() - 3, 3);

                int id = -1;
                if (it->first == "init_nexus") {}
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
        }
    }
    else
    {
        Pair m = *find_if
                (ob.buildList.begin(), ob.buildList.end(), [producedby](const Pair &p)
                 { return p.second == producedby; }
                );

        producerID = m.first;

        MapVec::iterator it = ob.buildingIDsMap.end(); --it;
        MapVec::iterator itbegin = ob.buildingIDsMap.begin(); --itbegin;
        do
        {
            string dec = it->first;
            dec.erase(dec.size() - 3, 3);

            int id = -1;
            if (it->first == "init_nexus") {}
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

/*void Simulation_P::run(Simulation_P *sim, Protoss *protoss, Parameter &param, string &actualEvent, ifstream &fin)
{

    sim->checkFinishedBuildings(*protoss, param);

    if (end == 0)
    {
        buildSuccess = sim->createCurrentBuild(actualEvent, *protoss, param);
        if (protoss->finish.size() > 0)
            for (vector<int>::iterator it = protoss->finish.begin(); it != protoss->finish.end(); ++it)
                sim->writeJson("build-end", *it, *protoss, param);
        if (protoss->start.size() > 0)
            for (int i = buildSuccess; i != 0; i = sim->createCurrentBuild(actualEvent, *protoss, param))
            {
                sim->writeJson("build-start", protoss->buildList[actualEvent], *protoss, param);
                fin >> actualEvent;
                if (fin.eof()) { end = 1; }
            }
    }
    else
    {
        if (!over)		buildSuccess = sim->createCurrentBuild(actualEvent, *protoss, param);

        if (protoss->finish.size() > 0)
            for (vector<int>::iterator it = protoss->finish.begin(); it != protoss->finish.end(); ++it)
                sim->writeJson("build-end", *it, *protoss, param);
        if (protoss->start.size() > 0)
            for (int i = buildSuccess; i != 0; i = sim->createCurrentBuild(actualEvent, *protoss, param))
            {
                sim->writeJson("build-start", protoss->buildList[actualEvent], *protoss, param);
                over = 1;							// means we started building the last item of buildlist
            }
        if (protoss->finish.size() > 0)
        {
            if (sim->checkCreated(*protoss) == 1 && over == 1)
            {
                param.wasJsonWritten = 2;
                error = 1;		// We've done!
            }
        }
    }
}*/

void Simulation_P::run(Simulation_P *sim, Protoss *protoss, Parameter &param, string &actualEvent, ifstream &fin)
{
    sim->checkFinishedBuildings(*protoss, param);

    if (end == 0)
    {
        buildSuccess = sim->createCurrentBuild(actualEvent, *protoss, param);
        if (protoss->finish.size() > 0)
            for (vector<int>::iterator it = protoss->finish.begin(); it != protoss->finish.end(); ++it)
                sim->writeJson("build-end", *it, *protoss, param);

        if (protoss->start.size() > 0)
            for (int i = buildSuccess; i != 0; i = sim->createCurrentBuild(actualEvent, *protoss, param))
            {
                sim->writeJson("build-start", protoss->buildList[actualEvent], *protoss, param);
                fin >> actualEvent;
                if (fin.eof()) { end = 1; }
            }
    }
    else
    {

        if (protoss->finish.size() > 0)
            for (vector<int>::iterator it = protoss->finish.begin(); it != protoss->finish.end(); ++it)
                sim->writeJson("build-end", *it, *protoss, param);

        if (protoss->finish.size() > 0)
        {
            if (sim->checkCreated(*protoss) == 1)
            {
                param.wasJsonWritten = 2;
                error = 1; // We've done!
            }
        }
    }
}