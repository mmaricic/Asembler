#include <iostream>
#include "MainProcessor.h"

int main(int argc, char *argv[]) {
	try {
		if (argc != 3) {
			cout << "Invalid number of command line arguments!";
			return -1;
		}
		string inputFile = argv[1];
		string otputFile = argv[2];
		MainProcessor handler;
		handler.resolvePassOne(inputFile);
		handler.resolvePassTwo(inputFile);
		handler.print(otputFile);

		return 0;
	}
	catch (HandleError e) {
		cout << e;
		return -1;
	}
	
}