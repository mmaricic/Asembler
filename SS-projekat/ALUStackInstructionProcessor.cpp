#include "ALUStackInstructionProcessor.h"



ALUStackInstructionProcessor::ALUStackInstructionProcessor()
{
	ALUStackOpcodes.insert(make_pair("PUSH","0x20"));
	ALUStackOpcodes.insert(make_pair("POP", "0x21"));
	ALUStackOpcodes.insert(make_pair("ADD", "0x30"));
	ALUStackOpcodes.insert(make_pair("SUB", "0x31"));
	ALUStackOpcodes.insert(make_pair("MUL", "0x32"));
	ALUStackOpcodes.insert(make_pair("DIV", "0x33"));
	ALUStackOpcodes.insert(make_pair("MOD", "0x34"));
	ALUStackOpcodes.insert(make_pair("AND", "0x35"));
	ALUStackOpcodes.insert(make_pair("OR", "0x36"));
	ALUStackOpcodes.insert(make_pair("XOR", "0x37"));
	ALUStackOpcodes.insert(make_pair("NOT", "0x38"));
	ALUStackOpcodes.insert(make_pair("ASL", "0x39"));
	ALUStackOpcodes.insert(make_pair("ASR", "0x3A"));
}

void ALUStackInstructionProcessor::resolvePassOne(string opcode)
{
	if (currentSection == "")
		throw HandleError("Invalid code - this must be inside of a section");
	vector<string> args = Parser::getArguments(line);
	if ((opcode == "PUSH" || opcode == "POP") && args.size() != 1)
			throw HandleError("Stack operations must have exactly 1 operand");
	else if(opcode != "PUSH" && opcode != "POP" && args.size() != 3)
		throw HandleError("ALU operations must have exactly 3 operands");
	for (string arg : args) 
		if (addressMode(arg) != REGDIR)
			throw HandleError("Address mode must be Register direct");
	
	locationCounter += 4;
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

