#include "DirectiveProcessor.h"


string DirectiveProcessor::getSectionType(string name)
{
	int pos = name.find('.', 1);
	return name.substr(0, pos);
}

void DirectiveProcessor::handleSection(string opcode)
{
	string section = getSectionType(opcode);
	string flags = wasORG? "O": "";
	//UBACI FLEGOVE i za ORG
	if (section == ".TEXT") {

	}
	else if (section == ".DATA") {

	}
	else if (section == ".RODATA") {

	}
	else if (section == ".BSS") {

	}
	else
		throw HandleError("Invalid section name");

	symTable->addSection(opcode, flags);
	locationCounter = ORG;
	currentSection = opcode;

}


void DirectiveProcessor::resolvePassOne(string opcode)
{
	if (opcode == ".GLOBAL") {
		if (wasORG)
			throw HandleError("After ORG can only be beginning of a section");

		vector<string> args = Parser::getArguments(line);
		if (args.size() == 0)
			throw HandleError(".global must have arguments");

		symTable->addGlobal(args);
		wasORG = false;
	}
	//SREDI ORG
	else if (opcode == "ORG") {
		if (wasORG)
			throw HandleError("After ORG can only be beginning of a section");
		vector<string> args = Parser::getArguments(line);
		if (args.size() != 1)
			throw HandleError("ORG must have exactly 1 paramater");
		ORG = ExpressionHandler::calculateConstant(args[0]);
		wasORG = true;
	}
	else {
		if(!wasORG)
			ORG = 0;
		handleSection(opcode);
		wasORG = false;
	}
}

void DirectiveProcessor::resolvePassTwo(string opcode)
{
	if (opcode != "ORG" && opcode != ".GLOBAL") {
		TableRow* section = symTable->getSymbol(opcode);
		if (section == nullptr)
			throw HandleError("Unexpected error");
		locationCounter = section->startAddress;
		currentSection = section->name;
	}
}
