#include "SymbolProcessor.h"


void SymbolProcessor::resolvePassOne(string opcode)
{
	string code = Parser::getNextWord(line);

	if (code != "DEF")
		throw new exception(); //unkown

	vector<string> expression = Parser::getArguments(line);
	if (expression.size() != 1)
		throw new exception(); //invalid expression format

	//ovde izracunaj vrednost izraza expression[0]
	//smesti u tabelu simbola ovaj simbol
}

void SymbolProcessor::resolvePassTwo(string opcode)
{
}
