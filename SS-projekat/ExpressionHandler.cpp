#include "ExpressionHandler.h"

SymbolTable* ExpressionHandler::table = SymbolTable::getInstance();


std::string ExpressionHandler::IntToHexString(int i, int width)
{
	std::stringstream stream;
	stream << "0x"
		/*<< std::setfill('0')*/ << std::setw(width)
		<< std::hex << i;
	return stream.str();
}
