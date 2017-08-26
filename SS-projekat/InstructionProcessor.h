#pragma once
#include "BaseProcessor.h"
#include "ExpressionHandler.h"



class InstructionProcessor :
	public BaseProcessor
{
	map<string, string> instructionOpcodes;
protected:
	InstructionProcessor();
	string addressModeToString(AddressType adr);
	AddressType addressMode(string arg);
	void calculateLC(string arg, string opcode);
	void printInsToSection(const string& objProgram, const string& opcode);
	string bitsForAddresPart(bool& bytes8, string arg, int& secondBytes, int& relFor, char& relType);

public:
	
};

