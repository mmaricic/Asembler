#include <iostream>
#include "Parser.h"

void main(int argc, char *argv[]) {

	string line = "opcode arg1, [arg2],[arg3 ]		 ,	 arg4 arg5 ";
	transform(line.begin(), line.end(), line.begin(), ::toupper);
	Parser::getNextWord(line);
	cout << line << endl;
	vector<string> args = Parser::getArguments(line);
	for (string arg : args)
		cout << "\"" << arg << "\"" << endl;
	
}