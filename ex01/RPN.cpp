#include <RPN.hpp>

RPN::RPN() {}

RPN::RPN(std::string const & expression)
{
    evaluate(expression);
}  

RPN::~RPN() {}

RPN::RPN(RPN const & copy)
{
    *this = copy;
}

RPN & RPN::operator=(RPN const & copy)
{
    if (this != &copy)
        this->_numbers = copy._numbers;
    return (*this);
}

bool RPN::isOperator(char c) const
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int RPN::performOperation(int a, int b, char op) const
{
    switch (op)
    {
        case '+': return (a + b);
        case '-': return (a - b);
        case '*': return (a * b);
        case '/': 
            if (b == 0)
            {
                throw std::runtime_error("Error: Division by zero.");
            }
            return (a / b);
        default:
            throw std::runtime_error("Error: Unknown operator.");
    }
}

void RPN::evaluate(std::string const & expression)
{
    for (size_t i = 0; i < expression.length(); ++i)
    {
        char token = expression[i];

        if (std::isspace(token))
            continue;

        if (std::isdigit(token))
        {
            _numbers.push(token - '0');
        }
        else if (isOperator(token))
        {
            if (_numbers.size() < 2)
            {
                throw std::runtime_error("Error: Insufficient values in the expression.");
            }

            int b = _numbers.top(); _numbers.pop();
            int a = _numbers.top(); _numbers.pop();
            int result = performOperation(a, b, token);
            _numbers.push(result);
        }
        else
        {
            throw std::runtime_error(std::string("Error: Invalid token '") + token + "'.");
        }
    }

    if (_numbers.size() != 1)
    {
        throw std::runtime_error("Error: The user input has too many values.");
    }

    std::cout << _numbers.top() << std::endl;
}
