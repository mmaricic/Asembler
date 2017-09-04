#include "LoadStoreInstructionProcessor.h"



string LoadStoreInstructionProcessor::getDataType(string& opcode)
{
	string dataType;

	if (opcode.find("load") != string::npos) {
		dataType = opcode.substr(4);
		opcode = "load";
	}
	else {
		dataType = opcode.substr(5);
		opcode = "store";
	}
	if (dataType == "")
		dataType = "dd";
	return dataType;
}


void LoadStoreInstructionProcessor::resolvePassOne(string opcode)
{
	if (State::currentSection == "")
		throw HandleError("Invalid code - this must be inside of a section");
	string dataType = getDataType(opcode);
	vector<string> args = Parser::getArguments(line);
	if (args.size() != 2)
		throw HandleError("LOAD and STORE instructions must have exactly 2 operands");

	if (addressMode(args[0]) != REGDIR)
		throw HandleError("First argument in LOAD and STORE instructions must be in Register direct address mode");

	calculateLC(args[1], opcode);
}


void LoadStoreInstructionProcessor::resolvePassTwo(string opcode)
{
	string dataType = getDataType(opcode);
	vector<string> args = Parser::getArguments(line);
	int secondBytes;
	int reallocationFor = -1;
	char relType;
	bool bytes8 = true;

	string objProgram = bitsForAddresPart(bytes8, args[1], secondBytes, reallocationFor, relType);
	objProgram = objProgram + commonOpcodes[args[0]] + "00000" + commonOpcodes[dataType] + "000";
	printInsToSection(objProgram, opcode);

	if (bytes8) 
		printExpToSection(secondBytes, reallocationFor, relType);
	
	
}
