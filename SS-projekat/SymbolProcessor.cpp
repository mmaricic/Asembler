#include "SymbolProcessor.h"


void SymbolProcessor::resolvePassOne(string opcode)
{
	string code = Parser::getNextWord(line);

	if (code != "def")
		throw HandleError("Undefined");

	vector<string> expression = Parser::getArguments(line);
	if (expression.size() != 1)
		throw HandleError("Constant symbol definition can have only 1 paramater");

	int section = -1;
 	int result = ExpressionHandler::calculate(expression[0], section);
	symTable->addSymbol(opcode, section, result);
}

void SymbolProcessor::resolvePassTwo(string opcode)
{
}
