#include "Parser.h"


void Parser::removeComments(string & line)
{
	regex reg("[^;]*");
	smatch result;
	if (regex_search(line, result, reg))
		line = result[0];
	reg = "(.*)[ \t]*";
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
		string word = (*iter)[1];
		line = (*iter)[2];
		return word;
	}
	else return nullptr;
}

vector<string> Parser::getArguments(string & line)
{
	regex reg("[^,]+");
	smatch result;
	vector<string> args;
	sregex_iterator iter(line.begin(), line.end(), reg);
	sregex_iterator end;
	while (iter != end)
	{
		string word = (*iter)[0];
		auto strBegin = word.find_first_not_of(" \t");
		if (strBegin == std::string::npos)
			strBegin = 0;
		
		auto strEnd = word.find_last_not_of(" \t");
		const auto strRange = strEnd - strBegin + 1;

		args.push_back(word.substr(strBegin, strRange));
		++iter;
	}
	return args;
}

bool Parser::isEnd(string line)
{
	transform(line.begin(), line.end(), line.begin(), ::toupper);
	regex reg("[ \t]*(\\.END)[ \t]*");
	return regex_match(line, reg);
}

