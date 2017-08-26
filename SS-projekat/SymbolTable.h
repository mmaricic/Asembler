#pragma once
#include <string>
#include <map>
#include <vector>
#include "State.h"
#include "HandleError.h"

using namespace std;

class SymbolTable
{
	static SymbolTable* instance;
	static unsigned int Ordinal;
	map<string, TableRow*> symbols;
	map<int, TableRow*> sortedSymbols;
	pair<map<string, TableRow*>::iterator, bool> ret;
	
	SymbolTable() {}
	SymbolTable(SymbolTable const&) = delete;

public:
	static SymbolTable* getInstance();

	TableRow* getSymbol(string key);
	void addLabels(vector<string> labels);
	void addSymbol(string key, int section, int value);
	void addGlobal(vector<string> keys);
	void addSection(string key, string flags);
	TableRow* getSection(int ordinal);
	void closeSection();

	~SymbolTable();
};

