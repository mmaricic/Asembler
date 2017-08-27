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
	sections[State::currentSection]->translatedProgram = sections[State::currentSection]->translatedProgram + instructionOpcodes[opcode] + (State::locationCounter % 16 == 0 ? "\n" : " ");
	for (string byte : bytes) {
		State::locationCounter++;
		sections[State::currentSection]->translatedProgram = sections[State::currentSection]->translatedProgram + byte + (State::locationCounter % 16 == 0 ? "\n" : " ");
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
			int strBegin = arg.find_first_not_of("$ \t");
			int strEnd = arg.find_last_not_of("$ \t");
			int strRange = strEnd - strBegin + 1;
			string argument = arg.substr(strBegin, strRange);
			int num;
			TableRow* elem = symTable->getSymbol(argument);
			if (ExpressionHandler::isNumber(argument, num)) {
				if (elem != nullptr) {
					if (elem->section != -1) {
						if (symTable->getSection(elem->section)->flags.find('O') != string::npos)
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
					secondBytes = elem->value + symTable->getSection(elem->section)->startAddress - 4;
					relType = 'R';
					relFor = elem->flags == "G" ? elem->ordinal : elem->section;
				}
			}
			reg0 = commonOpcodes["PC"];
		}
		else {
			int strBegin = arg.find_first_of("+-");
			int strEnd = arg.find_last_not_of("] \t");
			int strRange = strEnd - strBegin + 1;
			secondBytes = ExpressionHandler::calculate(arg.substr(strBegin, strRange), relFor);
			strBegin = arg.find_first_not_of("[ \t");
			strEnd = arg.find_first_of("+- \t", strBegin);
			strRange = strEnd - strBegin + 1;
			reg0 = arg.substr(strBegin, strRange);
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
	instructionOpcodes.insert(make_pair("PUSH","20"));
	instructionOpcodes.insert(make_pair("POP", "21"));
	instructionOpcodes.insert(make_pair("ADD", "30"));
	instructionOpcodes.insert(make_pair("SUB", "31"));
	instructionOpcodes.insert(make_pair("MUL", "32"));
	instructionOpcodes.insert(make_pair("DIV", "33"));
	instructionOpcodes.insert(make_pair("MOD", "34"));
	instructionOpcodes.insert(make_pair("AND", "35"));
	instructionOpcodes.insert(make_pair("OR", "36"));
	instructionOpcodes.insert(make_pair("XOR", "37"));
	instructionOpcodes.insert(make_pair("NOT", "38"));
	instructionOpcodes.insert(make_pair("ASL", "39"));
	instructionOpcodes.insert(make_pair("ASR", "3A"));

	instructionOpcodes.insert(make_pair("INT", "00"));
	instructionOpcodes.insert(make_pair("RET", "01"));
	instructionOpcodes.insert(make_pair("JMP", "02"));
	instructionOpcodes.insert(make_pair("CALL", "03"));
	instructionOpcodes.insert(make_pair("JZ", "04"));
	instructionOpcodes.insert(make_pair("JNZ", "05"));
	instructionOpcodes.insert(make_pair("JGZ", "06"));
	instructionOpcodes.insert(make_pair("JGEZ", "07"));
	instructionOpcodes.insert(make_pair("JLZ", "08"));
	instructionOpcodes.insert(make_pair("JLEZ", "09"));

	instructionOpcodes.insert(make_pair("LOAD", "10"));
	instructionOpcodes.insert(make_pair("STORE", "11"));
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
