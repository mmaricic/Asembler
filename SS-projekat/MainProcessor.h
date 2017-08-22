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
	map<string, BaseProcessor*> handlers;
	
public:
	MainProcessor();
	~MainProcessor();

	void resolvePassOne(string opcode) override;
	void resolvePassTwo(string opcode) =0;
};

