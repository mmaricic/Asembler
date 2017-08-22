#pragma once
#include "BaseProcessor.h"
class DataProcessor :
	public BaseProcessor
{
public:
	DataProcessor();
	~DataProcessor();

	map<string, int> dataSize;

	void resolvePassOne(string opcode) override;

	void resolvePassTwo(string opcode) override;
};

