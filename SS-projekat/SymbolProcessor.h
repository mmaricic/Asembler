#pragma once
#include "BaseProcessor.h"
#include "ExpressionHandler.h"

class SymbolProcessor :
	public BaseProcessor
{
public:
	void resolvePassOne(string opcode) override;

	void resolvePassTwo(string opcode) override;
};

