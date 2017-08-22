#pragma once
#include <string>
#include <map>
#include "SymbolTable.h"
#include "State.h"
#include "Parser.h"

using namespace std;

class BaseProcessor
{
protected:
	static map<string, string> commonOpcodes;
	static string line;
	SymbolTable* symTable = SymbolTable::getInstance();

	static map<string, Section*> sections;
public:
	virtual void resolvePassOne(string opcode) =0;
	virtual void resolvePassTwo(string opcode) = 0;
	BaseProcessor();
	virtual ~BaseProcessor();
};

