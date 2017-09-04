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
			sections[State::currentSection]->translatedProgram = sections[State::currentSection]->translatedProgram + num + ((State::locationCounter - State::ORG) % 16 == 0 ? "\n" : " ");
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
	
	commonOpcodes["r0"] = "00000";
	commonOpcodes["r1"] = "00001";
	commonOpcodes["r2"] = "00010";
	commonOpcodes["r3"] = "00011";
	commonOpcodes["r4"] = "00100";
	commonOpcodes["r5"] = "00101";
	commonOpcodes["r6"] = "00110";
	commonOpcodes["r7"] = "00111";
	commonOpcodes["r8"] = "01000";
	commonOpcodes["r9"] = "01001";
	commonOpcodes["r10"] = "01010";
	commonOpcodes["r11"] = "01011";
	commonOpcodes["r12"] = "01100";
	commonOpcodes["r13"] = "01101";
	commonOpcodes["r14"] = "01110";
	commonOpcodes["r15"] = "01111";
	commonOpcodes["sp"] = "10000";
	commonOpcodes["pc"] = "10001";

	commonOpcodes["dd"] = "000";
	commonOpcodes["uw"] = "001";
	commonOpcodes["sw"] = "101";
	commonOpcodes["ub"] = "011";
	commonOpcodes["sb"] = "111";
	commonOpcodes["b"] = "011";
	commonOpcodes["w"] = "001";
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
