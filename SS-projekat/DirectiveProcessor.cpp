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
		throw new exception(); //"Invalid section"

	symTable->addSection(opcode, flags);
	locationCounter = ORG;
	currentSection = opcode;

}


void DirectiveProcessor::resolvePassOne(string opcode)
{
	if (opcode == ".GLOBAL") {
		if (wasORG)
			throw new exception(); //after ORG must be section
		vector<string> args = Parser::getArguments(line);
		if (args.size() == 0)
			throw new exception();/*.global without arguments*/
		symTable->addGlobal(args);
		wasORG = false;
	}
	//SREDI ORG
	else if (opcode == "ORG") {
		if (wasORG)
			throw new exception(); //after ORG must be section
		vector<string> args = Parser::getArguments(line);
		if (args.size() != 1)
			throw new exception;//"invalid expression"
		//izracunaj konstantu
		wasORG = true;
		//dodeli org izracunatu vrednost
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
			throw new exception(); //something's wrong
		locationCounter = section->startAddress;
		currentSection = section->name;
	}
}


DirectiveProcessor::~DirectiveProcessor()
{
}
