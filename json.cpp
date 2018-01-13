#include "json.h"
#include "protoss.h"

void finishJson()
{
	ofstream json;
	json.open("stdout.json", fstream::app);
	json << "]\n},";
	json.close();
}

void finishJsonEnd()
{
	ofstream json;
	json.open("stdout.json", fstream::app);
	json << "]\n}";
	json.close();
}

void stdoutput()
{
	const char *file = "stdout.json";
	char ch;

	ifstream in(file);
	while (!in.eof())
	{
		ch = in.get();
		cout << ch;
	}
	in.close();
}

void OUT(Terran *ob, Parameter param) //for debugging!
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

void OUT(Protoss *ob, Parameter param) //for debugging!
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