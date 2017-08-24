#include "SymbolProcessor.h"


void SymbolProcessor::resolvePassOne(string opcode)
{
	string code = Parser::getNextWord(line);

	if (code != "DEF")
		throw HandleError("Undefined");

	vector<string> expression = Parser::getArguments(line);
	if (expression.size() != 1)
		throw HandleError("Constant symbol definition can have only 1 paramater");

	int relFor = -1;
	char relType;
	int result = ExpressionHandler::calculate(expression[0], relFor, relType);
	symTable->addSymbol(opcode, relFor, result);
}

void SymbolProcessor::resolvePassTwo(string opcode)
{
}
