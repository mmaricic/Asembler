#pragma once
#include "InstructionProcessor.h"

class LoadStoreInstructionProcessor :
	public InstructionProcessor
{
	string getDataType(string& opcode);
	
public:
	
	void resolvePassOne(string opcode) override;

	void resolvePassTwo(string opcode) override;
};

