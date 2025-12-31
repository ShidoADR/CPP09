#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include <cctype>
#include <vector>
#include <deque>

class	PmergeMe
{
    public:
							PmergeMe(std::string const &);

							~PmergeMe();

	private:
		std::vector<int>	_data;

							PmergeMe();
							PmergeMe(PmergeMe const &);
		PmergeMe &			operator=(PmergeMe const &);

};