#include <iostream>
#include "MainProcessor.h"

int main(int argc, char *argv[]) {
	try {
		if (argc != 2) {
			cout << "Invalid number of command line arguments!";
			return -1;
		}
			string inputFile = argv[1];
			auto pos = inputFile.find_last_of('.');
			string outputFile = inputFile.substr(0, pos) + "Obj" + (pos == string::npos ? "" : inputFile.substr(pos));
			MainProcessor handler;
			handler.resolvePassOne(inputFile);
			handler.resolvePassTwo(inputFile);
			handler.print(outputFile);
			
		return 0;
	}
	catch (HandleError e) {
		cout << e;
		return -1;
	}
	
}