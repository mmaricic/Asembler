#pragma once
#include <string>
#include <vector>

struct State {
	static unsigned int locationCounter;
	static std::string currentSection;
	static int ORG;
	static bool wasORG;
	static unsigned int dollar;

	static int lineNumber;
};

enum AddressType { INTERMEDIATE, REGDIR, REGIND, MEMDIR, REGINDOFF };

struct TableRow {
	std::string type;
	int ordinal;
	std::string name;
	int section;
	int startAddress;
	unsigned int size = 0;
	unsigned int value = 0;
	std::string flags;

	TableRow(std::string atype, std::string aname, int asection) : type(atype), name(aname), section(asection) {
		startAddress = 0;
		flags = "L";
	}

	TableRow(std::string atype, std::string aname) : type(atype), name(aname) {
	}

	TableRow(std::string atype,int aordinal,std::string aname,int asection,int astartAddress, unsigned int avalue,std::string aflags) {
		type = atype;
		 ordinal = aordinal;
		name = aname;
		 section = asection;
		 startAddress = astartAddress;
		  size = 0;
		  value = avalue;
		flags = aflags;
	}

	void setOrdinal(unsigned int ord) {
		ordinal = ord;
		if (type == "SEG")
			section = ord;
	}

};

enum Rel_Type { ABSOLUTE = 'A', PCRELATIVE = 'R' };

struct reallocation {
	
	int offset;
	Rel_Type type;
	int relativeTo;
};

struct Section {
	std::string translatedProgram;
	std::vector<reallocation> reallocations;
};