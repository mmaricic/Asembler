#include "DataFlowInstructionProcessor.h"


DataFlowInstructionProcessor::DataFlowInstructionProcessor()
{
	dataFlowOpcodes.insert(make_pair("INT", INT));
	dataFlowOpcodes.insert(make_pair("RET",RET));
	dataFlowOpcodes.insert(make_pair("JMP", UNCONDITIONAL));
	dataFlowOpcodes.insert(make_pair("CALL", UNCONDITIONAL));
	dataFlowOpcodes.insert(make_pair("JZ", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("JNZ", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("JGZ", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("JGEZ", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("JLZ", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("JLEZ", CONDITIONAL));
}

void DataFlowInstructionProcessor::resolvePassOne(string opcode)
{
	if (currentSection == "")
		throw HandleError("Invalid code - this must be inside of a section");
	dataFlowType type = dataFlowOpcodes[opcode].type;
	vector<string> args = Parser::getArguments(line);
	switch (type)
	{
	case INT:
		if (args.size() != 1 || (args.size() == 1 && addressMode(args[0]) != REGDIR))
			throw HandleError("Invalid format for INT instruction - it must have exactly 1 operand and it must be register direct address mode");
		locationCounter += 4;
		break;
	case RET:
		if (args.size() != 0)
			throw HandleError("RET instruction can't have arguments");		
		locationCounter += 4;
		break;
	case CONDITIONAL:
		if (args.size() != 2 || (args.size() == 2 && addressMode(args[0]) != REGDIR))
			throw HandleError("Conditional data flow instructions must have 2 arguments and 1 must be in register direct address mode");
		calculateLC(args[1], opcode);
		break;
	case UNCONDITIONAL:
		if (args.size() != 1)
			throw HandleError("Unconditional sata flow instructions must have exactly 1 argument");
		calculateLC(args[0], opcode);
		break;
	}
}

void DataFlowInstructionProcessor::resolvePassTwo(string opcode)
{
	string objProgram;

	if (opcode == "RET") {
		objProgram = string(24, '0');
		printInsToSection(objProgram, opcode);
		return;
	}

	dataFlowType type = dataFlowOpcodes[opcode].type;
	vector<string> args = Parser::getArguments(line);
	int secondBytes;
	int relocationFor = -1;
	char relType;
	bool bytes8 = true;

	switch (type)
	{
	case INT:
	case UNCONDITIONAL:
		objProgram = bitsForAddresPart(bytes8, args[0], secondBytes, relocationFor, relType);
		objProgram += string(16, '0');
		break;
	case CONDITIONAL:
		objProgram = bitsForAddresPart(bytes8, args[1], secondBytes, relocationFor, relType);
		objProgram = objProgram + commonOpcodes[args[0]] + string(11, '0');
		break;
	default:
		throw HandleError("Unexpected error");

	}

	printInsToSection(objProgram, opcode);

	
	if (bytes8) 
		printExpToSection(secondBytes, relocationFor, relType);
}


