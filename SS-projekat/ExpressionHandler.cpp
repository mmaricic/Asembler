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
	return 0;
}

int ExpressionHandler::calculate(string exp, int & relFor, char & relType)
{
	node* root = infixToPostfix(exp);
	if (firstPassCalculation(root))
		return root->number;
	//ovde uvedi razresavanje kad ne valja
	return 0;
}


node* ExpressionHandler::infixToPostfix(string infix) {
		stack<char> s;
		int weight;
		int i = 0;
		int k = 0;
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
				// simply push the opening parenthesis
				s.push(ch);
				i++;
				continue;
			}

			if (ch == ')') {
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
				i++;
				continue;
			}

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
					if (!isNumber(operand, num) && symTable->getSymbol(operand) == nullptr)
						throw HandleError("Invalid operand" + operand + " in constant expression!");
					node* temp = new node(operand, num);
					if (nodes.empty()) {
						if (s.size() == 1 && (s.top() == '-' || s.top() == '+')) {
							node* temp2 = new node(string(1, s.top()));
							s.pop();
							temp2->left =temp;
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
			addNode(s.top(), nodes);
			s.pop();
		}
		return nodes[0];
	}

	void ExpressionHandler::addNode(char ch, vector<node*>& nodes) {
		node* help = new node(string(1, ch));
		node* right = nodes.back();
		nodes.pop_back();
		node* left = nullptr;
		if (!nodes.empty()) {
			left = nodes.back();
			nodes.pop_back();
				
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
			return false;
	}

	bool ExpressionHandler::firstPassCalculation(node* root)
	{
		if (root->left == nullptr && root->right == nullptr)
			return root->symbol == "";
		bool left;
		bool right;
		if (root->left != nullptr)
			left = firstPassCalculation(root->left);
		if (root->right != nullptr)
			right = firstPassCalculation(root->right);
		else if (left) {
			if (root->symbol == "+")
				root->number = root->left->number;
			if (root->symbol == "-")
				root->number = 0 - root->left->number;
			
			delete root->left;
			root->left = nullptr;
			root->symbol = "";
			return true;
		}
		if (left && right) {
			
			if (root->symbol == "+")
				root->number = root->left->number + root->right->number;
			if (root->symbol == "-")
				root->number = root->left->number - root->right->number;
			if (root->symbol == "*")
				root->number = root->left->number * root->right->number;
			if (root->symbol == "/")
				root->number = root->left->number / root->right->number;

			delete root->left;
			delete root->right;
			root->left = nullptr;
			root->right = nullptr;
			root->symbol = "";
			return true;
		}
		return false;
	}
