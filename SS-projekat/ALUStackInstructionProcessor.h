#pragma once
#include "InstructionProcessor.h"
#include "ExpressionHandler.h"

class ALUStackInstructionProcessor :
	public InstructionProcessor
{
	
public:
	void resolvePassOne(string opcode) override;
	void resolvePassTwo(string opcode) override;

};

