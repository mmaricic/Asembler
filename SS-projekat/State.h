#pragma once
#include <string>
#include <vector>

static unsigned int locationCounter = 0;
static std::string currentSection = "";
enum AddressType { INTERMEDIATE, REGDIR, REGIND, MEMDIR, REGINDOFF};
static int ORG = 0;
static bool wasORG = false;
static unsigned int dollar = 0;

static int lineNumber = 0;

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

struct relocation {
	
	int offset;
	Rel_Type type;
	int relativeTo;
};

struct Section {
	std::string translatedProgram;
	std::vector<relocation> realocations;
};