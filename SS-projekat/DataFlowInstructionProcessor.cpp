#include "DataFlowInstructionProcessor.h"


DataFlowInstructionProcessor::DataFlowInstructionProcessor()
{
	dataFlowOpcodes.insert(make_pair("int", INT));
	dataFlowOpcodes.insert(make_pair("ret",RET));
	dataFlowOpcodes.insert(make_pair("jmp", UNCONDITIONAL));
	dataFlowOpcodes.insert(make_pair("call", UNCONDITIONAL));
	dataFlowOpcodes.insert(make_pair("jz", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("knz", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("jgz", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("jgez", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("jlz", CONDITIONAL));
	dataFlowOpcodes.insert(make_pair("jlez", CONDITIONAL));
}

void DataFlowInstructionProcessor::resolvePassOne(string opcode)
{
	if (State::currentSection == "")
		throw HandleError("Invalid code - this must be inside of a section");
	dataFlowType type = dataFlowOpcodes[opcode];
	vector<string> args = Parser::getArguments(line);
	switch (type)
	{
	case INT:
		if (args.size() != 1 || (args.size() == 1 && addressMode(args[0]) != REGDIR))
			throw HandleError("Invalid format for INT instruction - it must have exactly 1 operand and it must be register direct address mode");
		State::locationCounter += 4;
		break;
	case RET:
		if (args.size() != 0)
			throw HandleError("RET instruction can't have arguments");		
		State::locationCounter += 4;
		break;
	case CONDITIONAL:
		if (args.size() != 2 || (args.size() == 2 && addressMode(args[0]) != REGDIR))
			throw HandleError("Conditional data flow instructions must have 2 arguments and 1 must be in register direct address mode");
		calculateLC(args[1], opcode);
		break;
	case UNCONDITIONAL:
		if (args.size() != 1)
			throw HandleError("Unconditional data flow instructions must have exactly 1 argument");
		calculateLC(args[0], opcode);
		break;
	}
}

void DataFlowInstructionProcessor::resolvePassTwo(string opcode)
{
	string objProgram;

	if (opcode == "ret") {
		objProgram = string(24, '0');
		printInsToSection(objProgram, opcode);
		return;
	}

	dataFlowType type = dataFlowOpcodes[opcode];
	vector<string> args = Parser::getArguments(line);
	int secondBytes;
	int reallocationFor = -1;
	char relType;
	bool bytes8 = true;

	switch (type)
	{
	case INT:
	case UNCONDITIONAL:
		objProgram = bitsForAddresPart(bytes8, args[0], secondBytes, reallocationFor, relType);
		objProgram += string(16, '0');
		break;
	case CONDITIONAL:
		objProgram = bitsForAddresPart(bytes8, args[1], secondBytes, reallocationFor, relType);
		objProgram = objProgram + commonOpcodes[args[0]] + string(11, '0');
		break;
	default:
		throw HandleError("Unexpected error");

	}

	printInsToSection(objProgram, opcode);

	
	if (bytes8) 
		printExpToSection(secondBytes, reallocationFor, relType);
}


