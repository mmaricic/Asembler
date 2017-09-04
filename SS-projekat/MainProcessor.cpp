#include "MainProcessor.h"
#include <iostream>
#include <fstream>

map<string, BaseProcessor*> MainProcessor::handlers;

MainProcessor::MainProcessor()
{
	DataFlowInstructionProcessor* dataFlow = new DataFlowInstructionProcessor();
	ALUStackInstructionProcessor* ALU = new ALUStackInstructionProcessor();
	DataProcessor* data = new DataProcessor();
	DirectiveProcessor* directive = new DirectiveProcessor();
	LoadStoreInstructionProcessor* loadStore = new LoadStoreInstructionProcessor();
	SymbolProcessor* symbol = new SymbolProcessor();

	handlers.insert(make_pair("int", dataFlow));
	handlers.insert(make_pair("jmp", dataFlow));
	handlers.insert(make_pair("call", dataFlow));
	handlers.insert(make_pair("ret", dataFlow));
	handlers.insert(make_pair("jz", dataFlow));
	handlers.insert(make_pair("jnz", dataFlow));
	handlers.insert(make_pair("jgz", dataFlow));
	handlers.insert(make_pair("jgez", dataFlow));
	handlers.insert(make_pair("jlz", dataFlow));
	handlers.insert(make_pair("jlez", dataFlow));
	handlers.insert(make_pair("load", loadStore));
	handlers.insert(make_pair("loadub", loadStore));
	handlers.insert(make_pair("loadsb", loadStore));
	handlers.insert(make_pair("loaduw", loadStore));
	handlers.insert(make_pair("loadsw", loadStore));
	handlers.insert(make_pair("store", loadStore));
	handlers.insert(make_pair("storeb", loadStore));
	handlers.insert(make_pair("storew", loadStore));
	handlers.insert(make_pair("push", ALU));
	handlers.insert(make_pair("pop", ALU));
	handlers.insert(make_pair("add", ALU));
	handlers.insert(make_pair("sub", ALU));
	handlers.insert(make_pair("mul", ALU));
	handlers.insert(make_pair("div", ALU));
	handlers.insert(make_pair("mod", ALU));
	handlers.insert(make_pair("and", ALU));
	handlers.insert(make_pair("or", ALU));
	handlers.insert(make_pair("xor", ALU));
	handlers.insert(make_pair("not", ALU));
	handlers.insert(make_pair("asl", ALU));
	handlers.insert(make_pair("asr", ALU));
	handlers.insert(make_pair("db", data));
	handlers.insert(make_pair("dd", data));
	handlers.insert(make_pair("dw", data));
	handlers.insert(make_pair("org", directive));
	handlers.insert(make_pair(".global", directive));
	handlers.insert(make_pair(".text", directive));
	handlers.insert(make_pair(".data", directive));
	handlers.insert(make_pair(".rodata", directive));
	handlers.insert(make_pair(".bss", directive));
	handlers.insert(make_pair("SymbolHandler", new SymbolProcessor()));
}


MainProcessor::~MainProcessor()
{
	delete handlers["int"];
	delete handlers["load"];
	delete handlers["push"];
	delete handlers["db"];
	delete handlers["org"];
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
			transform(line.begin(), line.end(), line.begin(), ::tolower);
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
	State::ORG = 0;
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
			transform(line.begin(), line.end(), line.begin(), ::tolower);
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
	vector<string> printIt;
	string exp = stream.str();
	for (map<int, TableRow*>::iterator it = all->begin(); it != all->end(); ++it) {
		if ((*it).second->type == "SEG") {
			printIt.push_back((*it).second->name);
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
	for (string name:printIt) {
		if (sections.count(name) == 0)
			continue;
		Section* curr = sections[name];
		myfile << "#rel" << name << endl;
		for (reallocation rel : curr->reallocations) {
			stream << setfill('0') << setw(4) << std::hex << rel.offset;
			myfile << "0x" << stream.str() << " " << (char)rel.type << " " << rel.relativeTo << endl;
			stream.str("");
		}
		myfile << "<" << name << ">" << endl;
		myfile << curr->translatedProgram << endl << endl;
	}
	myfile << "#end";
	myfile.close();
}
