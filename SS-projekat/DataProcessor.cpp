#include "DataProcessor.h"



DataProcessor::DataProcessor()
{
	dataSize.insert(make_pair("DD", 4));
	dataSize.insert(make_pair("DW", 2));
	dataSize.insert(make_pair("DB", 1));
}


void DataProcessor::resolvePassOne(string opcode)
{
	if (currentSection == "")
		throw HandleError("Invalid code - this must be inside of a section");
	vector<string> args = Parser::getArguments(line);
	int number = 0;
	for (string arg : args) {
		auto dupPos = arg.find("DUP");
		if (dupPos == string::npos)
			number++;
		else {
			int num = ExpressionHandler::calculateConstant(arg.substr(0, dupPos));
			number += num;
		}
	}
	number = number * dataSize[opcode];
	locationCounter += number;
}

void DataProcessor::resolvePassTwo(string opcode)
{
	vector<string> args = Parser::getArguments(line);
	string expression;
	for (string arg : args) {
		int number = 1;
		auto dupPos = arg.find("DUP");
		if (dupPos != string::npos)
		{
			number = ExpressionHandler::calculateConstant(arg.substr(0, dupPos));
			expression = arg.substr(dupPos + 3);
		}
		else
			expression = arg;
		int relFor = -1;
		int result = ExpressionHandler::calculate(expression, relFor);
		printExpToSection(result, relFor, 'A', dataSize[opcode], number);
	}

}
