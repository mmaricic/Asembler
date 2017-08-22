#pragma once
#include "BaseProcessor.h"
#include <vector>
#include "Parser.h"

using namespace std;

class DirectiveProcessor :
	public BaseProcessor
{
	string getSectionType(string name);
	void handleSection(string opcode);

public:
	void resolvePassOne(string opcode) override;
	void resolvePassTwo(string opcode) override;
	~DirectiveProcessor();
};

