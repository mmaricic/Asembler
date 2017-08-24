#include "SymbolTable.h"

unsigned int TableRow::Ordinal = 0;
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

		return nullptr;

}

void SymbolTable::addLabels(vector<string> labels)
{
	TableRow* section = symbols.find(currentSection)->second;
	for (string lab : labels) {
		ret = symbols.insert(make_pair(lab, new TableRow(string("SYM"), lab, section->ordinal)));
		if (ret.second == false) {
			if(ret.first->second->flags != "G")
				throw HandleError("Label " + lab + " is already defined");
		}
		ret.first->second->value = locationCounter - section->startAddress;
	}
}

void SymbolTable::addSymbol(string key, int section, int value)
{
	ret = symbols.insert(make_pair(key, new TableRow(string("SYM"), key, section)));
	if (ret.second == false) {
		throw HandleError("Symbol " + key+ " is already defined!");
	}
	else
		ret.first->second->value = value;
		
}

void SymbolTable::addGlobal(vector<string> keys)
{
	for (string key : keys) {
		ret = symbols.insert(make_pair(key, new TableRow(string("SYM"), key, 0)));
		if (ret.second == false) {
			if (ret.first->second->flags != "L")
				throw HandleError("Symbol " + key + " is already declared as global");
		}
		else {
			ret.first->second->value = 0;
		}
		ret.first->second->flags = "G";
	}
	}
void SymbolTable::addSection(string key, string flags)
{
	closeSection();

	ret = symbols.insert(make_pair(key, new TableRow(string("SEG"), key)));
	if (ret.second == false) {
		throw HandleError("Section "+ key+ " already exists");
	}
	ret.first->second->flags = flags;
	ret.first->second->startAddress = ORG;


}

void SymbolTable::closeSection()
{
	if (!currentSection.empty()) {
		TableRow* current = symbols.find(currentSection)->second;
		current->size = locationCounter - current->startAddress;
	}
}

SymbolTable::~SymbolTable()
{
	for (auto itr = symbols.begin(); itr != symbols.end(); itr++)
		delete itr->second;
	
}
