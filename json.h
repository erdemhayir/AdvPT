#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "terran.h"
#include "parameter.h"

class Json {
public:
	Json() {};
	void finishJson();
	void finishJsonEnd();
	void stdoutput();
	void OUT(Terran *ob, Parameter param); //for debugging!
};