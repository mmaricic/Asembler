#include "InstructionProcessor.h"

AddressType InstructionProcessor::addressMode(string arg)
{
	if (commonOpcodes.count(arg) == 1)
		return REGDIR;
	if (arg[0] == '#') {
		return INTERMEDIATE;
	}
	if (arg[0] == '$') {
		return REGINDOFF;
	}
	if (arg[0] == '[' && arg[arg.size() - 1] == ']') {
		if (commonOpcodes.count(arg.substr(1, arg.size() - 2)))
			return REGIND;
		else if(arg.find_first_of("+-") != string::npos)
			return REGINDOFF;
			
		throw HandleError("Invalid number size - to big for the given format");
	}
	return MEMDIR;
}

void InstructionProcessor::calculateLC(string arg, string opcode)
{
	AddressType type = addressMode(arg);

	switch (type)
	{
	case INTERMEDIATE:
		if (opcode != "LOAD")
			throw HandleError("Instruction can't use intermediate address mode");
		else
			locationCounter += 8;
		break;
	case REGDIR:
	case REGIND:
		locationCounter += 4;
		break;
	case MEMDIR:
	case REGINDOFF:
		locationCounter += 8;
		break;
	}
}

void InstructionProcessor::printInsToSection(const string& objProgram, const string& opcode)
{
	vector<string> bytes = ExpressionHandler::binStringToHex(objProgram);
	if (sections.count(currentSection) == 0)
		sections.insert(make_pair(currentSection, new Section()));
	for (string byte : bytes) {
		locationCounter++;
		sections[currentSection]->translatedProgram = sections[currentSection]->translatedProgram + byte + (locationCounter % 16 == 0 ? "\n" : " ");
	}
	locationCounter++;
	sections[currentSection]->translatedProgram = sections[currentSection]->translatedProgram + commonOpcodes[opcode] + (locationCounter % 16 == 0 ? "\n" : " ");

}


string InstructionProcessor::bitsForAddresPart(bool& bytes8, string arg, int& secondBytes, int& relFor, char& relType)
{
	AddressType adrtype = addressMode(arg);
	string objProgram = commonOpcodes[addressModeToString(adrtype)];
	string reg0 = "00000";
	relType = 'A';
	switch (adrtype)
	{
	case REGDIR:
		bytes8 = false;
		reg0 = commonOpcodes[arg];
		break;
	case REGIND:
	{
		bytes8 = false;
		auto strBegin = arg.find_first_not_of("[ \t");
		auto strEnd = arg.find_last_not_of("[ \t");
		const auto strRange = strEnd - strBegin + 1;
		reg0 = commonOpcodes[arg.substr(strBegin, strRange)];
		break;
	}
	case INTERMEDIATE:
	{
		secondBytes = ExpressionHandler::calculate(arg.substr(1), relFor);
		break;
	}
	case REGINDOFF:
		if (arg[0] == '$') {
			int strBegin = arg.find_first_not_of(" \t");
			int strEnd = arg.find_last_not_of(" \t");
			int strRange = strEnd - strBegin + 1;
			string argument = arg.substr(arg.find_first_not_of(" \t"));
			int num;
			if (ExpressionHandler::isNumber(argument, num)) {
				secondBytes = num - 4;
				relType = 'R';
				relFor = 0;
			}
			else {
				TableRow* elem = symTable->getSymbol(argument);
				if (elem == nullptr)
					throw HandleError("Invalid argument with pc relative address mode");
				if (elem->type == "SEG")
					throw HandleError("Section can not be used as an argument!");
				secondBytes = elem->value - 4;
				relType = 'R';
				relFor = elem->flags == "G" ? elem->ordinal : elem->section;
			}
			reg0 = commonOpcodes["PC"];
		}
		else{
		int strBegin = arg.find_first_of("+-");
		int strEnd = arg.find_last_not_of("] \t");
		int strRange = strEnd - strBegin + 1;
		secondBytes = ExpressionHandler::calculateConstant(arg.substr(strBegin, strRange));
		strBegin = arg.find_first_not_of("[ \t");
		strEnd = arg.find_first_of("+- \t", strBegin);
		strRange = strEnd - strBegin + 1;
		reg0 = arg.substr(strBegin, strRange);
		break;
	}
	case MEMDIR:
	{
		secondBytes = ExpressionHandler::calculate(arg, relFor);
		break;
	}
	}
	return objProgram + reg0;
}


string InstructionProcessor::addressModeToString(AddressType adr)
{
	switch (adr)
	{
	case INTERMEDIATE:
		return "INTERMEDIATE";
	case REGDIR:
		return "REGDIR";
	case REGIND:
		return "REGIND";
	case MEMDIR:
		return "MEMDIR";
	case REGINDOFF:
		return "REGINDOFF";
	default:
		throw HandleError("Unexpected error");
	}
}
