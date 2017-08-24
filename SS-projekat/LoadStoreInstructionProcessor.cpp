#include "LoadStoreInstructionProcessor.h"



string LoadStoreInstructionProcessor::getDataType(string& opcode)
{
	string dataType;

	if (opcode.find("LOAD") != string::npos) {
		dataType = opcode.substr(4);
		opcode = "LOAD";
	}
	else {
		dataType = opcode.substr(5);
		opcode = "STORE";
	}
	if (dataType == "")
		dataType = "DD";
	return dataType;
}

LoadStoreInstructionProcessor::LoadStoreInstructionProcessor()
{
	opcodes.insert(make_pair("LOAD", "0x10"));
	opcodes.insert(make_pair("STORE", "0x11"));
}


void LoadStoreInstructionProcessor::resolvePassOne(string opcode)
{
	if (currentSection == "")
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
	int realocationFor = -1;
	char relType;
	bool bytes8 = true;

	string objProgram = bitsForAddresPart(bytes8, args[1], secondBytes, realocationFor, relType);
	objProgram = objProgram + commonOpcodes[args[0]] + "00000" + commonOpcodes[dataType] + "000";
	printInsToSection(objProgram, opcode);

	if (bytes8) 
		printExpToSection(secondBytes, realocationFor, relType);
	
	
}
