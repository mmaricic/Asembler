#include <iostream>
#include "Parser.h"

void main(int argc, char *argv[]) {
	string line = "tekst je ovo";
	cout << endl;
	cout << Parser::getNextWord(line) << endl;
	cout << line << endl;
	
}