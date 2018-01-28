#include "json.h"

void Json::finishJson()
{
	ofstream json;
	json.open("stdout.json", fstream::app);
	json << "]\n},";
	json.close();
}

void Json::finishJsonEnd()
{
	ofstream json;
	json.open("stdout.json", fstream::app);
	json << "]\n}";
	json.close();
}

void Json::stdoutput()
{
	ifstream f("stdout.json");

	if (f.is_open())
		cout << f.rdbuf();
}

void Json::OUT(Terran *ob, Parameter param) //for debugging!
{
	cout << "***" << param.Time << "***" << endl;
	// First row = what is being created (int from map)
	// Second row = time left
	for (int unsigned i = 0; i < ob->beingCreated.size(); ++i)
	{
		cout << ob->beingCreated[i][0];
		cout << "\t" << ob->beingCreated[i][1] << endl;
	}
	cout << "----------" << param.supplyUsed << "(" << param.totalSupCost << ")" << endl;
	cout << "----------" << param.minerals << "(" << param.vespene << ")" << endl;
}