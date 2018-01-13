#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "terran.h"
#include "protoss.h"
#include "parameter.h"

void finishJson();
void finishJsonEnd();
void stdoutput();
void OUT(Terran *ob, Parameter param); //for debugging!
void OUT(Protoss *ob, Parameter param); //for debugging!