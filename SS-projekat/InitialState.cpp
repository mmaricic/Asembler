#include "State.h"

unsigned int State::locationCounter = 0;
std::string State::currentSection = "";
int State::ORG = 0;
bool State::wasORG = false;
unsigned int State::dollar = 0;
int State::lineNumber = 0;
