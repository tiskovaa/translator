#pragma once

#include <iostream>
#include "stack_and_queue.h"
#include "term.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class Translator
{
private:

	string str;
	vector<Term*> postfix;
	vector<Term*> str_terms;
	bool LexicalAnalysis();
	bool SyntacticAnalysis();
	bool SyntacticAnalysis2();
	void ToPostfix();
	double Calculation();

public:

	Translator(string s) : str(s) {}
	~Translator()
	{
		for (size_t i = 0; i < str_terms.size(); i++)
		{
			delete str_terms[i];
		}
	}
	double solve()
	{
		if (!LexicalAnalysis())
		{
			throw "error in lexical analysis";
		}

		if ((SyntacticAnalysis()==false)||(SyntacticAnalysis2() == false))
		{
			throw "error in syntactic analysis";
		}

		ToPostfix();

		for (size_t i = 0; i < str.size(); i++)
		{
			cout << str[i];
		}

		cout << "\n";

		for (size_t i = 0; i < str_terms.size(); i++)
		{
			str_terms[i]->print();
		}
		cout << "\n";

		for (size_t i = 0; i < postfix.size(); i++)
		{
			postfix[i]->print();
		}
		cout << "\n";

		double d = Calculation();

		cout << d << "\n";

		return d;
	}
};

bool Translator::LexicalAnalysis()
{
	size_t i = 0;

	while (i < str.size())
	{
		if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
		{
			str_terms.push_back(new Operator(str[i]));
			i++;
		}
		else if (str[i] >= '0' && str[i] <= '9')
		{
			string::size_type sz;
			str_terms.push_back(new Number(stod(str.substr(i), &sz)));
			i += sz;
		}
		else if (str[i] == '(' || str[i] == ')')
		{
			str_terms.push_back(new Bracket(str[i]));
			i++;
		}
		else
		{
			return false;
		}
	}

	return true;
}
void Translator::ToPostfix()
{
	Stack<Term*> stack;

	for (size_t i = 0; i < str_terms.size(); i++)
	{
		switch (str_terms[i]->GetType())
		{
		case type::NUMBER:
		{
			postfix.push_back(str_terms[i]);
			break;
		}

		case type::OPENING_BRACKET:
		{
			stack.push(str_terms[i]);
			break;
		}

		case type::CLOSING_BRACKET:
		{
			while (stack.top()->GetType() != type::OPENING_BRACKET)
			{
				postfix.push_back(stack.top());
				stack.pop();
			}
			stack.pop();

			break;
		}

		case type::OPERATOR:
		{
			while (!stack.empty() && stack.top()->GetType() == type::OPERATOR && ((Operator*)(stack.top()))->Priority() >= ((Operator*)(str_terms[i]))->Priority())
			{
				postfix.push_back(stack.top());
				stack.pop();
			}
			stack.push(str_terms[i]);
		}
		break;
		}
	}
	while (!stack.empty())
	{
		postfix.push_back(stack.top());
		stack.pop();
	}
}
int Conformity(char s)
{
	if (s == '(')
	{
		return ')';
	}
}

bool Translator::SyntacticAnalysis()
{
	Stack<char> stack;

	for (size_t i = 0; i < str_terms.size(); i++)
	{
		Term* currentTerm = str_terms[i];
		switch (currentTerm->GetType())
		{
		case type::OPENING_BRACKET:
		{
			stack.push(((Bracket*)currentTerm)->get_op());
			break;
		}
		case type::CLOSING_BRACKET:
		{
			if (!stack.empty() && ((Bracket*)currentTerm)->get_op() == Conformity(stack.top()))
			{
				stack.pop();
			}
			else
			{
				return false;
			}
			break;
		}

		default:

			break;
		}
	}
	return stack.empty();
}
std::map<type, char> type_status{
	{NUMBER,0}, {OPERATOR, 1} , {OPENING_BRACKET,2},{CLOSING_BRACKET ,3}
};

bool Translator::SyntacticAnalysis2() {
	if((str_terms.front()->GetType()==CLOSING_BRACKET)||(str_terms.front()->GetType()==OPERATOR))
		return false; 
	char status = 0;
	status = type_status[str_terms[0]->GetType()];
	for (size_t i = 1; i < str_terms.size(); ++i) {
		if (status == 0 || status == 3) {
			if (type_status[str_terms[i]->GetType()] == 0 || type_status[str_terms[i]->GetType()] == 2) return false;
			else status = type_status[str_terms[i]->GetType()];
		}

		else {
			if ((type_status[str_terms[i]->GetType()] == 1) || (type_status[str_terms[i]->GetType()] == 3)) return false;
			else status = type_status[str_terms[i]->GetType()];
		}
	}
	if ((str_terms.back()->GetType() == OPERATOR) || (str_terms.back()->GetType() == OPENING_BRACKET)) return false;
	return true;
}

double Translator::Calculation() {
	Stack<double> stack_num;
	for (size_t i = 0; i < postfix.size(); i++)
	{
		switch (postfix[i]->GetType())
		{
		case type::NUMBER:
		{
			stack_num.push(((Number*)(postfix[i]))->value);
			break;
		}
		case type::OPERATOR:
		{
			double operand_1 = stack_num.top();
			stack_num.pop();
			double operand_2 = stack_num.top();
			stack_num.pop();
			switch (((Operator*)(postfix[i]))->value)
			{
			case '+':
			{
				stack_num.push(operand_2 + operand_1);
				break;
			}
			case '-':
			{
				stack_num.push(operand_2 - operand_1);
				break;
			}
			case '*':
			{
				stack_num.push(operand_2 * operand_1);
				break;
			}
			case '/':
			{
				if (operand_1 == 0.0)
				{
					throw "division by zero";
				}
				stack_num.push(operand_2 / operand_1);
				break;
			}
			}
			break;
		}
		}
	}
	return stack_num.top();
}