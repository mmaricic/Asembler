#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "SymbolTable.h"

using namespace std;

class ExpressionHandler
{
	static SymbolTable* table;
public:

	static std::string IntToHexString(int i, int width);
	static vector<string> binStringToHex(string binVal);
	static void calculate(string exp, int& result, int& relFor, char& relType);
};

