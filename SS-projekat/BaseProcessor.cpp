#include "BaseProcessor.h"

map<string, string> BaseProcessor::commonOpcodes;
string BaseProcessor::line = "";
map<string, Section*> BaseProcessor::sections;


void BaseProcessor::printExpToSection(int exp, int relFor, char relType, int size, int repetition)
{
	vector<string> res = ExpressionHandler::IntToHex(exp, size);
	if (sections.count(State::currentSection) == 0)
		sections.insert(make_pair(State::currentSection, new Section()));
	for (int i = 0; i < repetition; i++) {
		if (relFor != -1)
			insertreallocation(relType, relFor);
		for (string num : res) {
			State::locationCounter++;
			sections[State::currentSection]->translatedProgram = sections[State::currentSection]->translatedProgram + num + (State::locationCounter % 16 == 0 ? "\n" : " ");
		}

	}
}

void BaseProcessor::insertreallocation(char relType, int relFor)
{
	reallocation rel;
	rel.offset = State::locationCounter;
	rel.type = static_cast<Rel_Type> (relType);
	rel.relativeTo = relFor;
	sections[State::currentSection]->reallocations.push_back(rel);
}

BaseProcessor::BaseProcessor()
{
	commonOpcodes["INTERMEDIATE"] = "100";
	commonOpcodes["REGDIR"] = "000";
	commonOpcodes["MEMDIR"] = "110";
	commonOpcodes["REGIND"] = "010";
	commonOpcodes["REGINDOFF"] = "111";
	
	commonOpcodes["R0"] = "00000";
	commonOpcodes["R1"] = "00001";
	commonOpcodes["R2"] = "00010";
	commonOpcodes["R3"] = "00011";
	commonOpcodes["R4"] = "00100";
	commonOpcodes["R5"] = "00101";
	commonOpcodes["R6"] = "00110";
	commonOpcodes["R7"] = "00111";
	commonOpcodes["R8"] = "01000";
	commonOpcodes["R9"] = "01001";
	commonOpcodes["R10"] = "01010";
	commonOpcodes["R11"] = "01011";
	commonOpcodes["R12"] = "01100";
	commonOpcodes["R13"] = "01101";
	commonOpcodes["R14"] = "01110";
	commonOpcodes["R15"] = "01111";
	commonOpcodes["SP"] = "10000";
	commonOpcodes["PC"] = "10001";

	commonOpcodes["DD"] = "000";
	commonOpcodes["UW"] = "001";
	commonOpcodes["SW"] = "101";
	commonOpcodes["UB"] = "011";
	commonOpcodes["SB"] = "111";
	commonOpcodes["B"] = "011";
	commonOpcodes["W"] = "001";
}


BaseProcessor::~BaseProcessor()
{
	if (symTable != nullptr) {
		symTable = nullptr;
		for (auto iter = sections.begin(); iter != sections.end(); iter++) {
			delete iter->second;
			iter->second = nullptr;
		}
	}
	
}
