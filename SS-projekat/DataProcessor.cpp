#include "DataProcessor.h"



DataProcessor::DataProcessor()
{
	dataSize.insert(make_pair("DD", 4));
	dataSize.insert(make_pair("DW", 2));
	dataSize.insert(make_pair("DB", 1));
}


DataProcessor::~DataProcessor()
{
}

void DataProcessor::resolvePassOne(string opcode)
{
	if (currentSection == "")
		throw new exception(); //invalid code
	vector<string> args = Parser::getArguments(line);
	int number = 0;
	for (string arg : args) {
		auto dupPos = arg.find("DUP");
		if (dupPos == string::npos)
			number++;
		else {
			//izracunaj konstantu caluclateConstant(arg.substr(0, dupPos))
			//ako je konstanta, dodaj to na number, ako nije, baci gresku
		}
	}
	number = number * dataSize[opcode];
	locationCounter += number;
}

void DataProcessor::resolvePassTwo(string opcode)
{
	vector<string> args = Parser::getArguments(line);
	string expression;
	for (string arg : args) {
		int number = 1;
		auto dupPos = arg.find("DUP");
		if (dupPos != string::npos)
		{
			//izracunaj konstantu caluclateConstant(arg.substr(0, dupPos))
			//dodeli to promenljivoj number
			expression = arg.substr(dupPos + 3);
		}
		else
			expression = arg;
		/*izracunaj vrednost expression-a
		for(int i = 0; i < number; i++){
		ako je relokatibilno, napravi zapis o realokaciji
		izracunatu vrednost odstampaj u sekciju u onoj velicini koja je u 
		mapi u LITTLE ENDIAN formatu
		}
		*/
	}

}
