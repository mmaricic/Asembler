#pragma once
#include <string>
#include <iostream>

class HandleError {
	std::string error;
	int line;
public:
	HandleError(std::string e, int l = lineNumber): error(e), line(l) {}

	friend std::ostream& operator<<(std::ostream& os, const HandleError& e) {
		return os << "Line " << e.line << ":\t" << e.error << std::endl;
	}
};