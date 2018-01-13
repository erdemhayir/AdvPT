#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "terran.h"
#include "protoss.h"
#include "json.h"
#include "simulation.h"
#include "simulation_protoss.h"
#include "parameter.h"

using namespace std;

int main(int argc, char const *argv[])
{
    bool race = true; //true: Protoss False: Protoss
    if(!race){
        Parameter param;
        Terran *terran	= new Terran();

        //Protoss code seeing
        //Protoss *protoss = new Protoss();

        Simulation *sim = new Simulation();

        int takingTooLong = 0;

        ofstream json;
        json.open("stdout.json");
        json << "{\n   \"buildlistValid\" : \"1\",\n   \"game\": \"sc2-hots-terran\",\n   initialUnits: {\n     scv: [\n         \"init_scv_00\",\n         \"init_scv_01\",\n         \"init_scv_02\",\n         \"init_scv_03\",\n         \"init_scv_04\",\n         \"init_scv_05\"],\n      command_center: [\"init_command_center\"]},\n   \"messages\" : [\n";
        json.close();

        string actualEvent;
        ifstream fin;
        fin.open("terran.txt");
        fin >> actualEvent;

        // ---------- Loop ------------- //
        do
        {
            terran->start.clear();
            sim->buildSuccess = 0;
            param.wasJsonWritten = 0;
            ++takingTooLong;
            ++param.Time;
            param.minerals += param.workersInMinerals * 0.7;
            param.vespene += param.workersInVespene * 0.35;

            if (param.energyCollect)
            {
                if (param.energy < 200) param.energy += 0.5625;
                else param.energy = 200;
            }

            // ---------- SIMULATION ------------- //
            sim->run(sim, terran, param, actualEvent, fin);

            //OUT();	// prints the details each step

            if (param.wasJsonWritten == 1) finishJson();
            if (param.wasJsonWritten == 2) finishJsonEnd();

        } while (!sim->error && takingTooLong < 500);

        json.open("stdout.json", fstream::app);
        json << "]\n }";
        json.close();

        if (takingTooLong == 500)
            sim->error = -1;		// Buildlist is invalid

        switch (sim->error)
        {
            case 1:
                stdoutput();			// Buildlist is successful. Output in stdout.json
                break;
            default:
                remove("stdout.json");	// Buildlist is invalid
                cout << "{ \"game\": \"sc2-hots-terran\",\n\"buildlistValid\" : \"0\" \n}";
        }
        system("pause");
        return 0;
    } else{
        Parameter param;
        Protoss *protoss	= new Protoss();

        //Protoss code seeing
        //Protoss *protoss = new Protoss();

        Simulation_P *sim = new Simulation_P();

        int takingTooLong = 0;

        ofstream json;
        json.open("stdout.json");
        //json << "{\n   \"buildlistValid\" : \"1\",\n   \"game\": \"sc2-hots-protoss\",\n   initialUnits: {\n     probe: [\n         \"init_probe_00\",\n         \"init_probe_01\",\n         \"init_probe_02\",\n         \"init_probe_03\",\n         \"init_probe_04\",\n         \"init_probe_05\"],\n      command_center: [\"init_nexus\"]},\n   \"messages\" : [\n";
        json << "{\n \"buildlistValid\" : 1,\n \"game\": \"sc2-hots-protoss\",\n \"initialUnits\": {\n \"probe\": [\n \"init_probe_00\",\n \"init_probe_01\",\n \"init_probe_02\",\n \"init_probe_03\",\n \"init_probe_04\",\n \"init_probe_05\"],\n \"nexus\": [\"init_nexus\"]},\n \"messages\" : [\n";
        json.close();

        string actualEvent;
        ifstream fin;
        fin.open("protoss.txt");
        fin >> actualEvent;

        // ---------- Loop ------------- //
        do
        {
            protoss->start.clear();
            sim->buildSuccess = 0;
            param.wasJsonWritten = 0;
            ++takingTooLong;
            ++param.Time;
            param.minerals += param.workersInMinerals * 0.7;
            param.vespene += param.workersInVespene * 0.35;

            //TODO change this part
            if (param.energyCollect)
            {
                //TODO I changed it 200 to 100 cuz in protoss it is like that
                if (param.energy < 100) param.energy += 0.5625;
                else param.energy = 100;
            }

            // ---------- SIMULATION ------------- //
            sim->run(sim, protoss, param, actualEvent, fin);

            OUT(protoss,param);	// prints the details each step

            if (param.wasJsonWritten == 1) finishJson();
            if (param.wasJsonWritten == 2) finishJsonEnd();

        } while (!sim->error && takingTooLong < 1000);

        json.open("stdout.json", fstream::app);
        json << "]\n }";
        json.close();

        if (takingTooLong == 1000)
            sim->error = -1;		// Buildlist is invalid

        switch (sim->error)
        {
            case 1:
                stdoutput();			// Buildlist is successful. Output in stdout.json
                break;
            default:
                //remove("stdout.json");	// Buildlist is invalid
                cout << "{ \"game\": \"sc2-hots-protoss\",\n\"buildlistValid\" : \"0\" \n}";
        }
        //return 0;
    }

}