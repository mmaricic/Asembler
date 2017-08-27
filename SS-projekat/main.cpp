#include <iostream>
#include "MainProcessor.h"

void main(int argc, char *argv[]) {
	try {
		if (argc != 3) {
			cout << "Invalid number of command line arguments!";
			return;
		}
		string inputFile = argv[1];
		string otputFile = argv[2];
		MainProcessor handler;
		handler.resolvePassOne(inputFile);
		handler.resolvePassTwo(inputFile);
		handler.print(otputFile);
	}
	catch (HandleError e) {
		cout << e;
	}
	
}