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
			State::lineNumber++;
			if (line.size() == 0)
				continue;
			State::dollar = State::locationCounter;
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
			if (State::wasORG && Opcode[0] != '.')
				throw HandleError("After ORG can only be beginning of a section");

			if (handlers.count(Opcode) == 1)
				handlers[Opcode]->resolvePassOne(Opcode);
			else  if (Opcode[0] == '.') {
				int pos = Opcode.find('.', 1);
				string section = Opcode.substr(0, pos);
				if (handlers.count(section) == 1)
					handlers[section]->resolvePassOne(Opcode);
				else
					throw HandleError("Undefined");
			}
			else
				handlers["SymbolHandler"]->resolvePassOne(Opcode);
		}
		file.close();
		if (!Parser::isEnd(line))
			throw HandleError("File is missing an .end directive!");
		symTable->closeSection();
	}
	else 
		throw HandleError("Invalid file - opening failed");
	State::locationCounter = 0;
	State::lineNumber = 0;
	State::wasORG = false;
	State::currentSection = "";
}

void MainProcessor::resolvePassTwo(string opcode)
{
	ifstream file;
	file.open(opcode);
	if (file.is_open()) {

		while (getline(file, line) && !Parser::isEnd(line)) {
			State::lineNumber++;
			if (line.size() == 0)
				continue;
			transform(line.begin(), line.end(), line.begin(), ::toupper);
			Parser::removeComments(line);
			Parser::getLabels(line);
			if (line.size() == 0)
				continue;
			if (Parser::isEnd(line))
				break;
			string Opcode = Parser::getNextWord(line);
			if (handlers.count(Opcode) == 1)
				handlers[Opcode]->resolvePassTwo(Opcode);
			else  if (Opcode[0] == '.') {
				int pos = Opcode.find('.', 1);
				string section = Opcode.substr(0, pos);
				if (handlers.count(section) == 1)
					handlers[section]->resolvePassTwo(Opcode);
				else
					throw HandleError("Undefined");

			}
			else
				handlers["SymbolHandler"]->resolvePassTwo(Opcode);
		}
		file.close();
	}
	else
		throw HandleError("Invalid file - opening failed");

}

void MainProcessor::print(string filename)
{
	ofstream myfile;
	myfile.open(filename);
	myfile << "#TabelaSimbola" << endl;
	map<int, TableRow*>* all = symTable->getAllElements();
	std::stringstream stream;
	
	string exp = stream.str();
	for (map<int, TableRow*>::iterator it = all->begin(); it != all->end(); ++it) {
		if ((*it).second->type == "SEG") {
			myfile << (*it).second->type << " " << (*it).second->ordinal << " " << (*it).second->name << " " << (*it).second->section << " 0x";
			stream << setfill('0') << setw(2) << std::hex << (*it).second->startAddress;
			myfile << stream.str() << " 0x"; 
			stream.str("");
			stream << setfill('0') << setw(2) << std::hex << (*it).second->size;
			myfile << stream.str() << " " << (*it).second->flags << endl;
			stream.str("");
		}
		else {
			myfile << (*it).second->type << " " << (*it).second->ordinal << " " << (*it).second->name << " " << (*it).second->section << " 0x";
			stream << setfill('0') << setw(2) << std::hex << (*it).second->value;
			myfile << stream.str() << " " << (*it).second->flags << endl;
			stream.str("");
		}
	}

	myfile << endl;
	for (map<int, TableRow*>::iterator it = all->begin(); it != all->end(); ++it) {
		if ((*it).second->type == "SYM" || sections.count((*it).second->name) == 0)
			continue;
		Section* curr = sections[(*it).second->name];
		myfile << "#rel" << (*it).second->name << endl;
		for (reallocation rel : curr->reallocations) {
			stream << setfill('0') << setw(4) << std::hex << rel.offset;
			myfile << "0x" << stream.str() << " " << (char)rel.type << " " << rel.relativeTo << endl;
			stream.str("");
		}
		myfile << "<" << (*it).second->name << ">" << endl;
		myfile << curr->translatedProgram << endl;
	}
	myfile << "#end";
	myfile.close();
}
