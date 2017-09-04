#include "ALUStackInstructionProcessor.h"



void ALUStackInstructionProcessor::resolvePassOne(string opcode)
{
	if (State::currentSection == "")
		throw HandleError("Invalid code - this must be inside of a section");
	vector<string> args = Parser::getArguments(line);
	if ((opcode == "push" || opcode == "pop") && args.size() != 1)
			throw HandleError("Stack operations must have exactly 1 operand");
	else if(opcode != "push" && opcode != "pop" && args.size() != 3)
		throw HandleError("ALU operations must have exactly 3 operands");
	for (string arg : args) 
		if (addressMode(arg) != REGDIR)
			throw HandleError("Address mode must be Register direct");
	
	State::locationCounter += 4;
}

void ALUStackInstructionProcessor::resolvePassTwo(string opcode)
{
	string objProgram = "000";
	vector<string> args = Parser::getArguments(line);
	for (string arg : args)
		objProgram += commonOpcodes[arg];
	objProgram += string(24 - objProgram.length(), '0');
	printInsToSection(objProgram, opcode);
}

