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
		throw new exception(); //invalid code
	string dataType = getDataType(opcode);
	vector<string> args = Parser::getArguments(line);
	if (args.size() != 2)
		throw new exception(); //invalid arguments num

	if (addressMode(args[0]) != REGDIR)
		throw new exception(); //first argument must be regdir

	calculateLC(args[1], opcode);
}


void LoadStoreInstructionProcessor::resolvePassTwo(string opcode)
{
	string dataType = getDataType(opcode);
	vector<string> args = Parser::getArguments(line);
	AddressType adrtype = addressMode(args[1]);
	int secondBytes;
	int realocationFor = -100;
	char relType;
	bool bytes8 = true;

	string objProgram = bitsForAddresPart(adrtype, bytes8, args[1], secondBytes, realocationFor, relType);
	objProgram = objProgram + commonOpcodes[args[0]] + "00000" + commonOpcodes[dataType] + "000";
	printInsToSection(objProgram, opcode);

	if (realocationFor != -100) 
		insertRealocation(relType, realocationFor);
	if (bytes8)
		printExpToSection(secondBytes);
	
}
