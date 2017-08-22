#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using namespace std;

class Parser
{

public:
	static void removeComments(string& line);
	static vector<string> getLabels(string& line);
	static string getNextWord(string& line);
	static vector<string> getArguments(string& line);
	static bool isEnd(string line);

};

