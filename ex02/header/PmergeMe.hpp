#pragma once

#include <iostream>
#include <vector>
#include <deque>

class	PmergeMe
{
    public:
							PmergeMe();
							~PmergeMe();
							PmergeMe(PmergeMe const & copy);
		PmergeMe &			operator=(PmergeMe const & copy);

	private:
		std::vector<int>	_data;

};