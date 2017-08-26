#pragma once
#include "InstructionProcessor.h"
enum dataFlowType {INT, RET, CONDITIONAL, UNCONDITIONAL};


class DataFlowInstructionProcessor :
	public InstructionProcessor
{
	map<string, dataFlowType> dataFlowOpcodes;
public:
	DataFlowInstructionProcessor();
	void resolvePassOne(string opcode) override;

	void resolvePassTwo(string opcode) override;
};

