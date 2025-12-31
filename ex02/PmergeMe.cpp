#include <PmergeMe.hpp>

static bool	isNumber(std::string const & str)
{
	size_t	i = 0;
	
	if (str.empty() == true)
		return (false);

	if (str[0] == '+')
		i++;

	for (; i < str.length() && isdigit(str[i]) == true; i++);

	return (i == str.length());
}

PmergeMe::PmergeMe(){}

PmergeMe::~PmergeMe(){}

PmergeMe::PmergeMe(PmergeMe const & copy)
{
	*this = copy;
}

PmergeMe & PmergeMe::operator=(PmergeMe const & copy)
{
	if (this != &copy)
		this->_data = copy._data;

	return (*this);
}

PmergeMe::PmergeMe(std::string const & input)
{
	std::istringstream	ss(input);
	std::string			token;

	while (ss >> token)
	{
		if (isNumber(token) == false)
			throw std::invalid_argument("Error: " + token + "is not a positive Integer");

		long	num = std::strtoll(token.c_str(), NULL, 10);
		
		if (num > INT_MAX)
			throw std::out_of_range("Error: " + token + " is out of range");

		_data.push_back(static_cast<int>(num));
	}
}