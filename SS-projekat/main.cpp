#include <iostream>
#include "Parser.h"
#include "ExpressionHandler.h"

void main(int argc, char *argv[]) {
	try {
		int t;
		char d;
		int num = ExpressionHandler::calculate("-3+(2)*(4+(-10)-2)/(a-2)", t, d);
		cout << num << endl;
		/*string test = "010111110000001010100100";
	vector<string> res = ExpressionHandler::binStringToHex(test);
	for (string r : res)
		cout << r << " ";*/

		/*vector<string> res = ExpressionHandler::IntToHex(300, 2);
		for (string r : res)
			cout << r << " ";
		cout << endl;
		res = ExpressionHandler::IntToHex(-300, 1);
		for (string r : res)
			cout << r << " ";*/

		/*string line = "opcode";
		transform(line.begin(), line.end(), line.begin(), ::toupper);
		cout <<Parser::getNextWord(line) <<endl;
		cout << line << endl;
		vector<string> args = Parser::getArguments(line);
		for (string arg : args)
			cout << "\"" << arg << "\"" << endl;*/
	}
	catch (HandleError e) {
		cout << e;
	}
	
}