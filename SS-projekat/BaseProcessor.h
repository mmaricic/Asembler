#pragma once
#include <string>
#include <map>
#include "SymbolTable.h"
#include "State.h"
#include "Parser.h"
#include "ExpressionHandler.h"
#include "HandleError.h"
using namespace std;

class BaseProcessor
{
protected:
	static map<string, string> commonOpcodes;
	static string line;
	SymbolTable* symTable = SymbolTable::getInstance();
	static map<string, Section*> sections;

	void printExpToSection(int exp, int relFor, char relType, int size = 4, int repetition = 1);
	void insertreallocation(char relType, int relFor);

public:
	virtual void resolvePassOne(string opcode) =0;
	virtual void resolvePassTwo(string opcode) = 0;
	
	BaseProcessor();
	virtual ~BaseProcessor();
};

