#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <vector>
#include <deque>

class	PmergeMe
{
    public:
									PmergeMe(std::string const &);
		const std::vector<int> &	getData() const;

									~PmergeMe();
		
		std::vector<int>			mergeInsertVector(std::vector<int> const &);

	private:
		std::vector<int>			_data;
		std::deque<int>				_dequeData;

									PmergeMe();
									PmergeMe(PmergeMe const &);
		PmergeMe &					operator=(PmergeMe const &);

		void						makePair(std::vector<int> const &, std::vector<int> &, std::vector<int> &);
		void						updatePend(std::vector<int> &, std::vector<int> &, std::vector<int> &);
		std::vector<int>			jacobsthalSequence(size_t);
		// std::deque<int>				mergeInsertDeque(std::deque<int> const & data);

};

std::ostream &	operator<<(std::ostream &, PmergeMe const &);