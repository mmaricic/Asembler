#pragma once
#include "InstructionProcessor.h"
enum dataFlowType {INT, RET, CONDITIONAL, UNCONDITIONAL};

struct dataFlowValue {
	dataFlowType type;
	string opcode;
	dataFlowValue() {}
	dataFlowValue(dataFlowType t, string op): type(t), opcode(op) {}
};

class DataFlowInstructionProcessor :
	public InstructionProcessor
{
	map<string, dataFlowValue> dataFlowOpcodes;
public:
	DataFlowInstructionProcessor();
	void resolvePassOne(string opcode) override;

	void resolvePassTwo(string opcode) override;
};

