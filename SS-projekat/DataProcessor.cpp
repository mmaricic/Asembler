#include "DataProcessor.h"



DataProcessor::DataProcessor()
{
	dataSize.insert(make_pair("DD", 4));
	dataSize.insert(make_pair("DW", 2));
	dataSize.insert(make_pair("DB", 1));
}


void DataProcessor::resolvePassOne(string opcode)
{
	if (State::currentSection == "")
		throw HandleError("Invalid code - this must be inside of a section");
	vector<string> args = Parser::getArguments(line);
	int number = 0;
	int num = 0;
	bool dup = false;
	if (args.size() == 0)
		throw HandleError("Definition of data must have initial value");
	for (string arg : args) {
		auto dupPos = arg.find("DUP");
		if (dupPos == string::npos)
			if (dup)
				number += num;
			else
			number++;
		else {
			num = ExpressionHandler::calculateConstant(arg.substr(0, dupPos));
			if (num <= 0)
				throw HandleError("Number of repetition must be positive number greater then 0!");
			number += num;
			dup = true;
		}
	}
	number = number * dataSize[opcode];
	State::locationCounter += number;
}

void DataProcessor::resolvePassTwo(string opcode)
{
	vector<string> args = Parser::getArguments(line);
	string expression;
	bool dup = false;
	int number = 1;
	for (string arg : args) {
		auto dupPos = arg.find("DUP");
		if (dupPos != string::npos)
		{
			number = ExpressionHandler::calculateConstant(arg.substr(0, dupPos));
			dup = true;
			expression = arg.substr(dupPos + 3);
		}
		else {
			expression = arg;
			if(!dup)
				number = 1;
		}
		int relFor = -1;
		int result = 0;
		if (dup && expression.find_first_of("?") != string::npos)
			if (expression.find_first_not_of("? \t") != string::npos)
				throw HandleError("Invalid expression - if you use ? for initialisation value it has to be alone");
			else
				result = 0;
		else
			result = ExpressionHandler::calculate(expression, relFor);
		printExpToSection(result, relFor, 'A', dataSize[opcode], number);
	}

}
