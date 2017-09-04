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
		if (opcode != "load")
			throw HandleError("Instruction can't use intermediate address mode");
		else
			State::locationCounter += 8;
		break;
	case REGDIR:
	case REGIND:
		State::locationCounter += 4;
		break;
	case MEMDIR:
	case REGINDOFF:
		State::locationCounter += 8;
		break;
	}
}

void InstructionProcessor::printInsToSection(const string& objProgram, const string& opcode)
{
	vector<string> bytes = ExpressionHandler::binStringToHex(objProgram);
	if (sections.count(State::currentSection) == 0)
		sections.insert(make_pair(State::currentSection, new Section()));
	State::locationCounter++;
	sections[State::currentSection]->translatedProgram = sections[State::currentSection]->translatedProgram + instructionOpcodes[opcode] + ((State::locationCounter - State::ORG) % 16 == 0 ? "\n" : " ");
	for (string byte : bytes) {
		State::locationCounter++;
		sections[State::currentSection]->translatedProgram = sections[State::currentSection]->translatedProgram + byte + ((State::locationCounter - State::ORG) % 16 == 0 ? "\n" : " ");
	}
	
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
		auto strEnd = arg.find_last_not_of("] \t");
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
			int strBegin = arg.find_first_not_of("$ \t");
			int strEnd = arg.find_last_not_of("$ \t");
			int strRange = strEnd - strBegin + 1;
			string argument = arg.substr(strBegin, strRange);
			int num;
			TableRow* elem = symTable->getSymbol(argument);
			if (ExpressionHandler::isNumber(argument, num)) {
				if (elem != nullptr) {
					if (elem->section != -1) {
						if (symTable->getSymbol(State::currentSection)->flags.find('O') != string::npos)
							secondBytes = num - 8 - State::locationCounter;
						else {
							secondBytes = elem->value + symTable->getSection(elem->section)->startAddress - 4;
							relType = 'R';
							relFor = elem->flags == "G" ? elem->ordinal : elem->section;
						}
					}
					else {
						secondBytes = elem->value - 4;
						relType = 'R';
						relFor = elem->ordinal;
					}
				}
				else
					throw HandleError("You can't have pcrel address mode with constant number!");

			}
			else {
				if (elem == nullptr)
					throw HandleError("Invalid argument with pc relative address mode");
				if (elem->type == "SEG")
					throw HandleError("Section can not be used as an argument!");
				TableRow* sec = symTable->getSymbol(State::currentSection);
				if (elem->section == sec->ordinal)
					secondBytes = elem->value + sec->startAddress - 8 - State::locationCounter;
				else {
					secondBytes = (elem->flags == "G" ? 0 :elem->value + symTable->getSection(elem->section)->startAddress) - 4;
					relType = 'R';
					relFor = elem->flags == "G" ? elem->ordinal : elem->section;
				}
			}
			reg0 = commonOpcodes["pc"];
		}
		else {
			int strBegin = arg.find_first_of("+-");
			int strEnd = arg.find_last_not_of("] \t");
			int strRange = strEnd - strBegin + 1;
			secondBytes = ExpressionHandler::calculate(arg.substr(strBegin, strRange), relFor);
			strBegin = arg.find_first_not_of("[ \t");
			strEnd = arg.find_first_of("+- \t", strBegin);
			strRange = strEnd - strBegin;
			reg0 = commonOpcodes[arg.substr(strBegin, strRange)];
		}

		break;
	
	case MEMDIR:
	{
		secondBytes = ExpressionHandler::calculate(arg, relFor);
		break;
	}
	}
	return objProgram + reg0;
}


InstructionProcessor::InstructionProcessor()
{
	instructionOpcodes.insert(make_pair("push","20"));
	instructionOpcodes.insert(make_pair("pop", "21"));
	instructionOpcodes.insert(make_pair("add", "30"));
	instructionOpcodes.insert(make_pair("sub", "31"));
	instructionOpcodes.insert(make_pair("mul", "32"));
	instructionOpcodes.insert(make_pair("div", "33"));
	instructionOpcodes.insert(make_pair("mod", "34"));
	instructionOpcodes.insert(make_pair("and", "35"));
	instructionOpcodes.insert(make_pair("or", "36"));
	instructionOpcodes.insert(make_pair("xor", "37"));
	instructionOpcodes.insert(make_pair("not", "38"));
	instructionOpcodes.insert(make_pair("asl", "39"));
	instructionOpcodes.insert(make_pair("asr", "3A"));

	instructionOpcodes.insert(make_pair("int", "00"));
	instructionOpcodes.insert(make_pair("ret", "01"));
	instructionOpcodes.insert(make_pair("jmp", "02"));
	instructionOpcodes.insert(make_pair("call", "03"));
	instructionOpcodes.insert(make_pair("jz", "04"));
	instructionOpcodes.insert(make_pair("jnz", "05"));
	instructionOpcodes.insert(make_pair("jgz", "06"));
	instructionOpcodes.insert(make_pair("jgez", "07"));
	instructionOpcodes.insert(make_pair("jlz", "08"));
	instructionOpcodes.insert(make_pair("jlez", "09"));

	instructionOpcodes.insert(make_pair("load", "10"));
	instructionOpcodes.insert(make_pair("store", "11"));
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
