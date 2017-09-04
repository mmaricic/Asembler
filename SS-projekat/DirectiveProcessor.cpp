#include "DirectiveProcessor.h"


string DirectiveProcessor::getSectionType(string name)
{
	int pos = name.find('.', 1);
	return name.substr(0, pos);
}

void DirectiveProcessor::handleSection(string opcode)
{
	string section = getSectionType(opcode);
	string flags = State::wasORG? "O": "";
	//UBACI FLEGOVE i za ORG
	if (section == ".text") {
		flags += "APX";
	}
	else if (section == ".data") {
		flags += "APW";
	}
	else if (section == ".rodata") {
		flags += "AP";
	}
	else if (section == ".bss") {
		flags += "AW";
	}
	else
		throw HandleError("Invalid section name");

	symTable->addSection(opcode, flags);
	State::locationCounter = State::ORG;
	State::currentSection = opcode;

}


void DirectiveProcessor::resolvePassOne(string opcode)
{
	if (opcode == ".global") {
		if (State::wasORG)
			throw HandleError("After ORG can only be beginning of a section");

		vector<string> args = Parser::getArguments(line);
		if (args.size() == 0)
			throw HandleError(".global must have arguments");

		symTable->addGlobal(args);
		State::wasORG = false;
	}
	//SREDI ORG
	else if (opcode == "org") {
		if (State::wasORG)
			throw HandleError("After ORG can only be beginning of a section");
		vector<string> args = Parser::getArguments(line);
		if (args.size() != 1)
			throw HandleError("ORG must have exactly 1 paramater");
		State::ORG = ExpressionHandler::calculateConstant(args[0]);
		State::wasORG = true;
	}
	else {
		if(!State::wasORG)
			State::ORG = 0;
		handleSection(opcode);
		State::wasORG = false;
	}
}

void DirectiveProcessor::resolvePassTwo(string opcode)
{
	if (opcode != "org" && opcode != ".global") {
		TableRow* section = symTable->getSymbol(opcode);
		if (section == nullptr)
			throw HandleError("Unexpected error");
		State::locationCounter = section->startAddress;
		State::currentSection = section->name;
		State::ORG = section->startAddress;
	}
}
