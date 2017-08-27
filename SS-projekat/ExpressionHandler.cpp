#include "ExpressionHandler.h"
#include <algorithm>
#include <vector>

SymbolTable* ExpressionHandler::symTable = SymbolTable::getInstance();


vector<string> ExpressionHandler::IntToHex(int num, int width)
{
	vector<string> ret;
	std::stringstream stream;
	stream << setfill('0') << setw(width*2) << std::hex << num;
	string exp = stream.str();
	if (exp.length() > width * 2)
		if (num < 0) {
			int j = 0;
			while (j + 1 < exp.size() && exp[j] == exp[j + 1] && exp[j] == 'f' && exp.size() - j > width*2)
				j++;
			exp = exp.substr(j);
			if(exp.length() > width * 2)
				throw HandleError("Number is to big for the format");
		}
		else
			throw HandleError("Number is to big for the format");

	if (exp.length() < width * 2)
		exp = string(width * 2 - exp.length(), exp[0]) + exp;
	
	int i = width * 2 - 2;
	while (i >= 0) {
		ret.push_back(exp.substr(i, 2));
		i -= 2;
	}

	return ret;
}

vector<string> ExpressionHandler::binStringToHex(string binVal)
{
	vector<string> res;
	int one = stoul(binVal.substr(0, 8), 0, 2);
	int two = stoul(binVal.substr(8, 8), 0, 2);
	int three = stoul(binVal.substr(16), 0, 2);
	std::stringstream stream1;
	stream1 << setfill('0') << setw(2) <<std::hex << one;
	res.push_back(stream1.str());
	std::stringstream stream2;
	stream2 << setfill('0') << setw(2) << std::hex << two;
	res.push_back(stream2.str());
	std::stringstream stream3;
	stream3 << setfill('0') << setw(2) << std::hex << three;
	res.push_back(stream3.str());

	return res;
}

int ExpressionHandler::calculateConstant(string exp)
{
	node* root = infixToPostfix(exp);
	
	if (firstPassCalculation(root))
		return root->number;
	
	throw HandleError("Constant expression of this instruction can't bre reallocatable");
	
}

int ExpressionHandler::calculate(string exp, int & relFor)
{
	
	node* root = infixToPostfix(exp);
	
	if (firstPassCalculation(root))
			return root->number;
	
	if (root->symbol[0] == '-')
		throw HandleError("reallocation symbol can only be added to expression");
	if (root->symbol.find_first_of("+-", 1) != string::npos)
		throw HandleError("There can only be one reallocation symbol per expression");
	

	TableRow * symbol = symTable->getSymbol(root->symbol);
	
	if (symbol->type == "SEG")
		throw HandleError("Sections can not be used in expressions!");
	
	relFor = symbol->flags == "G" ? symbol->ordinal : symbol->section;
	int val = symbol->value + symTable->getSection(symbol->section)->startAddress;
	 return val + root->number;
	
}


node* ExpressionHandler::infixToPostfix(string infix) {
	if (infix[infix.find_first_not_of(" \t")] == '-' || infix[infix.find_first_not_of(" \t")] == '+')
		infix = "0" + infix;
	int i = 0;
	while (i < infix.size()) {
		while (i < infix.size() && infix[i] != '(')
			i++;
		if (i < infix.size()) {
			if (infix[infix.find_first_not_of(" \t", i+1)] == '-' || infix[infix.find_first_not_of(" \t", i + 1)] == '+')
				infix = infix.substr(0, i + 1) + "0" + infix.substr(i + 1);
			i++;
		}
	}
	stack<char> s;
		int weight = -2;
		i = 0;
		int symbolNum = 0;
		char ch;
		string operand = "";
		vector<node*> nodes;
		
		// iterate over the infix expression   
		while (i < infix.length()) {
			ch = infix[i];
			if (ch == ' ' || ch == '\t') {
				i++;
				continue;
			}

			if (ch == '(') {
				if (!validOrder(3, weight))
					throw HandleError("Invalid expression");
				// simply push the opening parenthesis
				s.push(ch);
				i++;
				weight = 3;
				continue;
			}

			if (ch == ')') {
				if (!validOrder(4, weight))
					throw HandleError("Invalid expression");
				// if we see a closing parenthesis,
				// pop of all the elements and append it to
				// the postfix expression till we encounter
				// a opening parenthesis
				while (!s.empty() && s.top() != '(') {
					addNode(s.top(), nodes);
					s.pop();

				}
				// pop off the opening parenthesis also
				if (!s.empty()) {
					s.pop();
				}
				else
					throw HandleError("Closing parenthesis doesn't have paired opening!");
				i++;
				weight = 4;
				continue;
			}
			if(operand == "")
				if (!validOrder(getWeight(ch), weight))
					throw HandleError("Invalid expression");

			weight = getWeight(ch);
			
			if (weight == -1)
				throw HandleError("Invalid character in constant expression!");

			if (weight == 0) {
				if (i + 1 < infix.size() && (isalnum(infix[i + 1]) || infix[i + 1] == '_')) {
					operand += ch;
					i++;
					continue;
				}
				else {
					// we saw an operand
					operand += ch;
					int num = 0;
					bool isNum = isNumber(operand, num);
					if (!isNum && symTable->getSymbol(operand) == nullptr)
						throw HandleError("Invalid operand" + operand + " in constant expression!");
					else if (!isNum)
						symbolNum++;
					node* temp = new node(operand, num);
					if (nodes.empty()) {
						if (s.size() == 1 && (s.top() == '-' || s.top() == '+')) {
							node* temp2 = new node(string(1, s.top()));
							s.pop();
							temp2->left =temp;
							temp->parent = temp2;
							temp = temp2;
						}
					}
					nodes.push_back(temp);
					operand = "";
					
				}

			}
			else {
				// we saw an operator
				if (s.empty()) {
					// simply push the operator onto stack if
					// stack is empty
					s.push(ch);
				}
				else {
					// pop of all the operators from the stack and
					// append it to the postfix expression till we
					// see an operator with a lower precedence that
					// the current operator
					while (!s.empty() && s.top() != '(' &&
						weight <= getWeight(s.top())) {
						addNode(s.top(), nodes);
						s.pop();
					}
					// push the current operator onto stack
					s.push(ch);
				}
			}
			i++;
		}
		// pop of the remaining operators present in the stack
		// and append it to postfix expression 
		while (!s.empty()) {
			if (s.top() == '(')
				throw HandleError("Openin parenthesis doesn't have paired closing");
			addNode(s.top(), nodes);
			s.pop();
		}
		node* root = nodes[0];
		return root;
	}

	bool ExpressionHandler::validOrder(int current, int before)
	{
		switch (current) {
		case 0:
			if (before == 3 || before == -2 || before == 2 || before == 1)
				return true;
			return false;
		case 3:
			if (before == -2 || before == 2 || before == 1 || before == 3)
				return true;
			return false;
		case 4:
			if (before == 0 || before == 4)
				return true;
			return false;
		case 2:
		case 1:
			if (before == 4 || before == 0)
				return true;
			return false;
		
		}
	}

	int ExpressionHandler::subtractSymbols(node * left, node * right)
	{
		vector<TableRow*> positive;
		vector<TableRow*> negative;
		auto i = right->symbol.find_first_of("+-");
		if (i == string::npos)
			negative.push_back(symTable->getSymbol(right->symbol));
		else {
			while (i != string::npos) {
				auto j = right->symbol.find_first_of("+-", i + 1);
				if (j == string::npos)
					j = left->symbol.length();
				TableRow* sym = symTable->getSymbol(right->symbol.substr(i + 1, j - i - 1));
				if (right->symbol[i] == '-')
					positive.push_back(sym);
				else
					negative.push_back(sym);
				i = right->symbol.find_first_of("+-", i + 1);
			}
		}

		i = left->symbol.find_first_of("+-");
		if (i == string::npos)
			positive.push_back(symTable->getSymbol(left->symbol));
		else {
			while (i != string::npos) {
				auto j = left->symbol.find_first_of("+-", i + 1);
				if (j == string::npos)
					j = left->symbol.length();
				TableRow* sym = symTable->getSymbol(left->symbol.substr(i + 1, j - i - 1));
				if (left->symbol[i] == '-')
					negative.push_back(sym);
				else
					positive.push_back(sym);
			}
		}
		int result = 0;
		bool found = false;
		for (vector<TableRow*>::iterator iter1 = positive.begin(); iter1 != positive.end();) {
			for (vector<TableRow*>::iterator iter2 = negative.begin(); iter2 != negative.end();) {
				if ((*iter1)->section == (*iter2)->section) {
					result = result + ((*iter1)->value - (*iter2)->value);
					iter1 = positive.erase(iter1);
					iter2 = negative.erase(iter2);
					found = true;
					break;
				}
				else 
					iter2++;
			}
			if (found == true)
				found = false;
			else
				iter1++;
		}
		string newExpression = "";
		int k;
		for (k = 0; k > (positive.size() < negative.size() ? positive.size() : negative.size()); k++)
			newExpression = newExpression + "+" + (positive[k])->name + "-" + (negative[k])->name;
		if (positive.size() > k)
			for (int j = k; j > positive.size(); j++)
				newExpression = newExpression + "+" + positive[j]->name;

		for (; k > negative.size(); k++)
			newExpression = newExpression + "-" + negative[k]->name;

		left->symbol = newExpression;
		return result;
	}

	void ExpressionHandler::addNode(char ch, vector<node*>& nodes) {
		node* help = new node(string(1, ch));
		node* right = nodes.back();
		nodes.pop_back();
		right->parent = help;
		node* left = nullptr;
		if (!nodes.empty()) {
			left = nodes.back();
			nodes.pop_back();
			left->parent = help;
		}	
		else {
			left = right;
			right = nullptr;
		}
		help->left = left;
		help->right = right;
		nodes.push_back(help);
	}

	int ExpressionHandler::getWeight(char ch) {
		switch (ch) {
		case '/':
		case '*': return 2;
		case '+':
		case '-': return 1;
		default: 
			if (isalnum(ch) || ch == '_' || ch == '$')
				return 0;
			else
				return -1;
		}
	}

	bool ExpressionHandler::isNumber(string& val, int& number)
	{
		string allowed = "0123456789";
		int start = 0;
		int base = 10;
		if(val[0] == '0'){
			switch (val[1]) {
			case 'B': 
				allowed = "01";
				start = 2;
				base = 2;
				break;
			
			case 'X': 
				allowed = "0123456789ABCDEF";
				start = 2;
				base = 16;
				break;
			
			default: 
				allowed = "01234567";
				start = 1;
				base = 8;
				break;
			
			}
		}
			auto pos = val.find_first_not_of(allowed, start);
			if (pos == string::npos) {
				if (base == 2)
					val = val.substr(2);
				number = stoi(val, nullptr, base);
				val = "";
				return true;
			}
			TableRow* sym = symTable->getSymbol(val);
			if (sym != nullptr) {
				TableRow* getn = symTable->getSection(sym->section);
				if (sym->section == -1 || (sym->section > 0 && getn != nullptr && getn->flags.find_first_of("O") != string::npos)) {
					number = sym->value + (getn == nullptr? 0 : getn->startAddress);
					val = "";
					return true;
				}
			}
			return false;
	}

	bool ExpressionHandler::firstPassCalculation(node* root)
	{
		if (root->left == nullptr && root->right == nullptr)
			return root->symbol == "";
		//bool left;
		//bool right;
		if (root->left != nullptr)
			/*left = */firstPassCalculation(root->left);
		if (root->right != nullptr)
			/*right = */firstPassCalculation(root->right);

		//if (left && right) {

		if (root->symbol == "+") {
			root->number = root->left->number + root->right->number;
			if (root->left->symbol == "" || root->right->symbol == "")
				root->symbol = root->left->symbol + root->right->symbol;
			else
				root->symbol = root->left->symbol + root->symbol + root->right->symbol;
		}
		
		else if (root->symbol == "-") {
			root->number = root->left->number - root->right->number;
			if (root->right->symbol == "")
				root->symbol = root->left->symbol;
			else {
				int res = subtractSymbols(root->left, root->right);
				root->number += res;
				root->symbol = root->left->symbol;
			}
		}
		else {
			if (root->left->symbol != "" || root->right->symbol != "")
				throw HandleError("Invalid expression - reallocatable symbols can't be multiplied or divided by anything");
			
			if (root->symbol == "*")
				root->number = root->left->number * root->right->number;
			
			else if (root->symbol == "/") {
				root->number = root->left->number / root->right->number;
			}
		}
			delete root->left;
			delete root->right;
			root->left = nullptr;
			root->right = nullptr;
			if(root->symbol == "")
			return true;
		//}
		return false;
	}
