#include "MainProcessor.h"
#include <iostream>
#include <fstream>


MainProcessor::MainProcessor()
{
	DataFlowInstructionProcessor* dataFlow = new DataFlowInstructionProcessor();
	ALUStackInstructionProcessor* ALU = new ALUStackInstructionProcessor();
	DataProcessor* data = new DataProcessor();
	DirectiveProcessor* directive = new DirectiveProcessor();
	LoadStoreInstructionProcessor* loadStore = new LoadStoreInstructionProcessor();
	SymbolProcessor* symbol = new SymbolProcessor();

	handlers.insert(make_pair("INT", dataFlow));
	handlers.insert(make_pair("JMP", dataFlow));
	handlers.insert(make_pair("CALL", dataFlow));
	handlers.insert(make_pair("RET", dataFlow));
	handlers.insert(make_pair("JZ", dataFlow));
	handlers.insert(make_pair("JNZ", dataFlow));
	handlers.insert(make_pair("JGZ", dataFlow));
	handlers.insert(make_pair("JGEZ", dataFlow));
	handlers.insert(make_pair("JLZ", dataFlow));
	handlers.insert(make_pair("JLEZ", dataFlow));
	handlers.insert(make_pair("LOAD", loadStore));
	handlers.insert(make_pair("LOADUB", loadStore));
	handlers.insert(make_pair("LOADSB", loadStore));
	handlers.insert(make_pair("LOADUW", loadStore));
	handlers.insert(make_pair("LOADSW", loadStore));
	handlers.insert(make_pair("STORE", loadStore));
	handlers.insert(make_pair("STOREB", loadStore));
	handlers.insert(make_pair("STOREW", loadStore));
	handlers.insert(make_pair("PUSH", ALU));
	handlers.insert(make_pair("POP", ALU));
	handlers.insert(make_pair("ADD", ALU));
	handlers.insert(make_pair("SUB", ALU));
	handlers.insert(make_pair("MUL", ALU));
	handlers.insert(make_pair("DIV", ALU));
	handlers.insert(make_pair("MOD", ALU));
	handlers.insert(make_pair("AND", ALU));
	handlers.insert(make_pair("OR", ALU));
	handlers.insert(make_pair("XOR", ALU));
	handlers.insert(make_pair("NOT", ALU));
	handlers.insert(make_pair("ASL", ALU));
	handlers.insert(make_pair("ASR", ALU));
	handlers.insert(make_pair("DB", data));
	handlers.insert(make_pair("DD", data));
	handlers.insert(make_pair("DW", data));
	handlers.insert(make_pair("ORG", directive));
	handlers.insert(make_pair(".GLOBAL", directive));
	handlers.insert(make_pair(".TEXT", directive));
	handlers.insert(make_pair(".DATA", directive));
	handlers.insert(make_pair(".RODATA", directive));
	handlers.insert(make_pair(".BSS", directive));
	handlers.insert(make_pair("SymbolHandler", new SymbolProcessor()));
}


MainProcessor::~MainProcessor()
{
	delete handlers["INT"];
	delete handlers["LOAD"];
	delete handlers["PUSH"];
	delete handlers["DB"];
	delete handlers["ORG"];
	delete handlers["SymbolHandler"];
}

void MainProcessor::resolvePassOne(string opcode)
{
	ifstream file;
	file.open(opcode);
	if (file.is_open()) {
		
		while (getline(file, line) && !Parser::isEnd(line)) {
			lineNumber++;
			if (line.size() == 0)
				continue;
			transform(line.begin(), line.end(), line.begin(), ::toupper);
			Parser::removeComments(line);
			vector<string> labels = Parser::getLabels(line);
			if (labels.size() != 0)
				symTable->addLabels(labels);
			if (line.size() == 0)
				continue;
			if (Parser::isEnd(line))
				break;
			string Opcode = Parser::getNextWord(line);
			if (wasORG && Opcode[0] != '.')
				throw new exception(); //after ORG must come section
			
			if (handlers.count(Opcode) == 1)
				handlers[Opcode]->resolvePassOne(Opcode);
			else  if (Opcode[0] == '.') {
				int pos = Opcode.find('.', 1);
				string section = Opcode.substr(0, pos);
				if (handlers.count(section) == 1)
					handlers[section]->resolvePassOne(Opcode);
				else
					throw new exception(); // invalid operation
			}
			else
				handlers["SymbolHandler"]->resolvePassOne(Opcode);
		}
		file.close();
		if (!Parser::isEnd(line))
			throw new exception(); //file doesn't have an end
		symTable->closeSection();
	}
	else 
		throw new exception(); //invalid file
	
	locationCounter = 0;
	lineNumber = 0;
	wasORG = false;
	currentSection = "";
}
