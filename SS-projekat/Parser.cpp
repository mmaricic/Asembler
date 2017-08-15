#include "Parser.h"
#include <regex>
#include <iostream>

void Parser::removeComments(string & line)
{
	regex reg("[^;]*");
	smatch result;
	if (regex_search(line, result, reg))
		line = result[0];
	reg = "(.*)\\s";
	if (regex_match(line, result, reg))
		line = result[1];
}

vector<string> Parser::getLabels(string & line)
{
	regex reg("(\\w+)(?::[ \t]*)");
	smatch result;
	vector<string> labels;
	sregex_iterator iter(line.begin(), line.end(), reg);
	sregex_iterator end;
	while (iter != end)
	{
			labels.push_back((*iter)[1]);
			++iter;
	}
	reg = "(?:.*):(?:[ \t])*(.*)";
	if (regex_match(line, result, reg))
		line = result[1];
	return labels;
}

string Parser::getNextWord(string & line)
{
	regex reg("(?:([^ \t]+)[ \t]*(.*))");
	smatch result;
	vector<string> labels;
	sregex_iterator iter(line.begin(), line.end(), reg);
	sregex_iterator end;
	if(iter != end) {
		for (int i = 0; i < iter->size(); i++)
			cout << (*iter)[i] <<" "+i << endl;
		line = (*iter)[2];
		return (*iter)[1];
	}
	else return NULL;
}

