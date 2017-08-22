#pragma once
#include "InstructionProcessor.h"
#include "ExpressionHandler.h"

class ALUStackInstructionProcessor :
	public InstructionProcessor
{
	map<string, string> ALUStackOpcodes;
	
public:
	ALUStackInstructionProcessor();
	void resolvePassOne(string opcode) override;
	void resolvePassTwo(string opcode) override;

};

