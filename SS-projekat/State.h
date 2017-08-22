#pragma once
#include <string>
#include <vector>

static unsigned int locationCounter = 0;
static std::string currentSection = "";
enum AddressType { INTERMEDIATE, REGDIR, REGIND, MEMDIR, REGINDOFF};
static int ORG = 0;
static bool wasORG = false;

static int lineNumber = 0;

struct TableRow {
	static unsigned int Ordinal;
	std::string type;
	int ordinal;
	std::string name;
	int section;
	int startAddress;
	unsigned int size = 0;
	unsigned int value = 0;
	std::string flags;

	TableRow(std::string atype, std::string aname, int asection) : type(atype), name(aname), section(asection) {
		ordinal = ++Ordinal;
		startAddress = 0;
		flags = "L";
	}

	TableRow(std::string atype, std::string aname) : type(atype), name(aname) {
		ordinal = ++Ordinal;
		section = ordinal;
	}

};

struct realocation {
	enum Rel_Type {ABSOLUTE = 'A', PCRELATIVE = 'R' };
	int offset;
	Rel_Type type;
	int relativeTo;
};

struct Section {
	std::string translatedProgram;
	std::vector<realocation> realocations;
};