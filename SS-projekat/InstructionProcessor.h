#pragma once
#include "BaseProcessor.h"
#include "ExpressionHandler.h"



class InstructionProcessor :
	public BaseProcessor
{
protected:
	string addressModeToString(AddressType adr);
	AddressType addressMode(string arg);
	void calculateLC(string arg, string opcode);
	void printInsToSection(const string& objProgram, const string& opcode);
	void printExpToSection(int exp);
	void insertRealocation(char relType, int relFor);
	string bitsForAddresPart(AddressType adrtype, bool& bytes8, string arg, int& secondBytes, int& relFor, char& relType);

public:
	
};

