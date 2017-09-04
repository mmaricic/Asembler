#include "SymbolTable.h"
#include "MainProcessor.h"

unsigned int SymbolTable::Ordinal = 0;
SymbolTable* SymbolTable::instance = nullptr;

SymbolTable * SymbolTable::getInstance()
{
		if (instance == nullptr) {
			static SymbolTable something;
			instance = &something;
		}
		return instance;
}

TableRow * SymbolTable::getSymbol(string key)
{
	if (symbols.count(key) > 0)
		return symbols[key];

	if (key == "$") {
		if (State::currentSection == "")
			return nullptr;
		dollarSymbol->value = State::dollar;
		dollarSymbol->section = symbols.find(State::currentSection)->second->ordinal;
		return dollarSymbol;
	}

	return nullptr;

}

void SymbolTable::addLabels(vector<string> labels)
{
	TableRow* section = symbols.find(State::currentSection)->second;
	for (string lab : labels) {
		if (MainProcessor::isOpcode(lab))
			throw HandleError("Keyword can't be used as a label");
		TableRow* newsym = new TableRow(string("SYM"), lab, section->ordinal);
		ret = symbols.insert(make_pair(lab, newsym));
		if (ret.second == false) {
			delete newsym;
			if (ret.first->second->flags != "G")
				throw HandleError("Label " + lab + " is already defined");
			else
				ret.first->second->section = section->ordinal;
		}
		else {
			newsym->setOrdinal(++Ordinal);

			sortedSymbols.insert(make_pair(Ordinal, newsym));
		}
		ret.first->second->value = State::locationCounter - section->startAddress;
	}
}

void SymbolTable::addSymbol(string key, int section, int value)
{
	if (MainProcessor::isOpcode(key))
		throw HandleError("Keyword can't be used as a symbol name");
	TableRow* newsym = new TableRow(string("SYM"), key, section);
	ret = symbols.insert(make_pair(key, newsym));
	if (ret.second == false) {
		delete newsym;
		throw HandleError("Symbol " + key+ " is already defined!");
	}
	else {
		newsym->setOrdinal(++Ordinal);
		ret.first->second->value = value;
		sortedSymbols.insert(make_pair(Ordinal, newsym));
	}
		
}

void SymbolTable::addGlobal(vector<string> keys)
{
	for (string key : keys) {
		if (MainProcessor::isOpcode(key))
			throw HandleError("Keyword can't be used as a global");
		TableRow* newsym = new TableRow(string("SYM"), key, 0);
		ret = symbols.insert(make_pair(key, newsym));
		if (ret.second == false) {
			delete newsym;
			if (ret.first->second->flags != "L")
				throw HandleError("Symbol " + key + " is already declared as global");
		}
		else {
			newsym->setOrdinal(++Ordinal);
			ret.first->second->value = 0;
			sortedSymbols.insert(make_pair(Ordinal, newsym));
		}
		ret.first->second->flags = "G";
	}
	}
void SymbolTable::addSection(string key, string flags)
{
	closeSection();
	TableRow* newsym = new TableRow(string("SEG"), key);
	ret = symbols.insert(make_pair(key,newsym));
	if (ret.second == false) {
		delete newsym;
		throw HandleError("Section "+ key+ " already exists");
	}
	else {
		newsym->setOrdinal(++Ordinal);
		sortedSymbols.insert(make_pair(Ordinal, newsym));
	}
	ret.first->second->flags = flags;
	ret.first->second->startAddress = State::ORG;


}

TableRow * SymbolTable::getSection(int ordinal)
{
	if (sortedSymbols.count(ordinal) > 0) {
		TableRow* res = sortedSymbols[ordinal];
		if (res->type == "SYM" && res->flags != "G")
			throw HandleError("Symbol must be section");
		return res;
	}
	return nullptr;

}

void SymbolTable::closeSection()
{
	if (!State::currentSection.empty()) {
		TableRow* current = symbols.find(State::currentSection)->second;
		current->size = State::locationCounter - current->startAddress;
	}
}

SymbolTable::~SymbolTable()
{
	delete dollarSymbol;
	for (auto itr = symbols.begin(); itr != symbols.end(); itr++) {
		if(itr->second != nullptr)
			delete itr->second;
		itr->second = nullptr;
	}
	
}
