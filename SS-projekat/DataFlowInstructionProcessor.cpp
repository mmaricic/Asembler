#include "DataFlowInstructionProcessor.h"


DataFlowInstructionProcessor::DataFlowInstructionProcessor()
{
	dataFlowOpcodes.insert(make_pair("INT", dataFlowValue(INT, "0x00")));
	dataFlowOpcodes.insert(make_pair("RET", dataFlowValue(RET, "0x01")));
	dataFlowOpcodes.insert(make_pair("JMP", dataFlowValue(UNCONDITIONAL, "0x02")));
	dataFlowOpcodes.insert(make_pair("CALL", dataFlowValue(UNCONDITIONAL, "0x03")));
	dataFlowOpcodes.insert(make_pair("JZ", dataFlowValue(CONDITIONAL, "0x04")));
	dataFlowOpcodes.insert(make_pair("JNZ", dataFlowValue(CONDITIONAL, "0x05")));
	dataFlowOpcodes.insert(make_pair("JGZ", dataFlowValue(CONDITIONAL, "0x06")));
	dataFlowOpcodes.insert(make_pair("JGEZ", dataFlowValue(CONDITIONAL, "0x07")));
	dataFlowOpcodes.insert(make_pair("JLZ", dataFlowValue(CONDITIONAL, "0x08")));
	dataFlowOpcodes.insert(make_pair("JLEZ", dataFlowValue(CONDITIONAL, "0x09")));
}

void DataFlowInstructionProcessor::resolvePassOne(string opcode)
{
	if (currentSection == "")
		throw new exception(); //invalid code
	dataFlowType type = dataFlowOpcodes[opcode].type;
	vector<string> args = Parser::getArguments(line);
	switch (type)
	{
	case INT:
		if (args.size() != 1 || (args.size() == 1 && addressMode(args[0]) != REGDIR))
			throw new exception(); //invalid INT instruction
		locationCounter += 4;
		break;
	case RET:
		if (args.size() != 0)
			throw new exception(); //RET cant have arguments
		locationCounter += 4;
		break;
	case CONDITIONAL:
		if (args.size() != 2 || (args.size() == 2 && addressMode(args[0]) != REGDIR))
			throw new exception(); //invalid CONDITIONAL instruction
		calculateLC(args[1], opcode);
		break;
	case UNCONDITIONAL:
		if (args.size() != 1)
			throw new exception(); //invalid argument num for unconditional jump
		calculateLC(args[0], opcode);
		break;
	}
}

void DataFlowInstructionProcessor::resolvePassTwo(string opcode)
{
}


