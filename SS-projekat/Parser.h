#pragma once
#include <string>
#include <vector>

using namespace std;

class Parser
{

public:
	static void removeComments(string& line);
	static vector<string> getLabels(string& line);
	static string getNextWord(string& line);
	static string getOpcode(string& line);
	static vector<string> getArguments(string& line);

};

