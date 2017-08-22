#pragma once
#include "InstructionProcessor.h"

class LoadStoreInstructionProcessor :
	public InstructionProcessor
{
	string getDataType(string& opcode);
	map<string, string> opcodes;
	
public:
	LoadStoreInstructionProcessor();

	void resolvePassOne(string opcode) override;

	void resolvePassTwo(string opcode) override;
};

