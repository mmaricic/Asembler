#pragma once
#include "BaseProcessor.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "ALUStackInstructionProcessor.h"
#include "DataFlowInstructionProcessor.h"
#include "DataProcessor.h"
#include "DirectiveProcessor.h"
#include "LoadStoreInstructionProcessor.h"
#include "SymbolProcessor.h"

class MainProcessor :
	public BaseProcessor
{
	static map<string, BaseProcessor*> handlers;
	
public:
	MainProcessor();
	~MainProcessor();

	void resolvePassOne(string opcode) override;
	void resolvePassTwo(string opcode) override;
	static bool isOpcode(string opcode) { 
		if (commonOpcodes.count(opcode) == 1) {
			if (commonOpcodes[opcode].length() == 5 || opcode == "dd")
				return true;
			else
				return false;
		}
		return handlers.count(opcode) == 1;
	}
	void print(string filename);
};

