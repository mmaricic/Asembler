#pragma once
#include "BaseProcessor.h"
class DataProcessor :
	public BaseProcessor
{
	map<string, int> dataSize;

public:
	DataProcessor();
	
	void resolvePassOne(string opcode) override;

	void resolvePassTwo(string opcode) override;
};

