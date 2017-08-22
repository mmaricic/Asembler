#pragma once
#include "BaseProcessor.h"
class SymbolProcessor :
	public BaseProcessor
{
public:
	void resolvePassOne(string opcode) override;

	void resolvePassTwo(string opcode) override;
};

