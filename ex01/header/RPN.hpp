#pragma once

#include <iostream>
#include <sstream>
#include <stack>

class RPN
{
	public:
						RPN(std::string const &);
						~RPN();
						RPN(RPN const &);
		RPN &			operator=(RPN const &);

		void			evaluate(std::string const &);
	
	private:
		std::stack<int>	_numbers;

						RPN();
		bool			isOperator(char c) const;
		int				performOperation(int a, int b, char op) const;
};