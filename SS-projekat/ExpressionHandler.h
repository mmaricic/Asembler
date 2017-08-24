#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stack>
#include "SymbolTable.h"

using namespace std;

struct node{
	string symbol = "";
	int number;
	node* left = nullptr;
	node* right = nullptr;

	node(string v, int n = 0): symbol(v), number(n) {}
};

class ExpressionHandler
{
	static SymbolTable* symTable;
	
	static void addNode(char ch, vector<node*>& nodes);
	static int getWeight(char ch);
	static bool isNumber(string& val, int& number);
	static bool firstPassCalculation(node* root);
	static node* infixToPostfix(string infix);
public:
	
	static vector<string> IntToHex(int num, int width);
	static vector<string> binStringToHex(string binVal);
	static int calculateConstant(string exp);
	static int calculate(string exp, int& relFor, char& relType);
};

