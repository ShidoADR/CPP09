#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <list>

class	PmergeMe
{
    public:
									PmergeMe(std::string const &);
		
									~PmergeMe();

		const std::vector<int> &	getData() const;
		const std::list<int> &		getListData() const;
		const std::vector<int> &	getSortedData() const;
		const std::list<int> &		getSortedListData() const;
		double						getTimeForVector() const;
		double						getTimeForList() const;

	private:
		std::vector<int>			_data;
		std::list<int>				_listData;
		std::vector<int>			_sortedData;
		std::list<int>				_sortedListData;
		double						_timeForList;
		double						_timeForVector;

									PmergeMe();
									PmergeMe(PmergeMe const &);
		PmergeMe &					operator=(PmergeMe const &);

		std::vector<int>			jacobsthalSequence(size_t);
		std::vector<int>			mergeInsertVector(std::vector<int> const &);
		void						updatePend(std::vector<int> &, std::vector<int> &, std::vector<int> &);
		void						makePair(std::vector<int> const &, std::vector<int> &, std::vector<int> &);

		std::list<int>				jacobsthalSequenceList(size_t);
		std::list<int>				mergeInsertList(std::list<int> const &);
		void						updatePend(std::list<int> &, std::list<int> &, std::list<int> &);
		void						makePair(std::list<int> const &, std::list<int> &, std::list<int> &);
};

std::ostream &						operator<<(std::ostream &, PmergeMe const &);
bool								isSorted(std::vector<int> const &);
bool								isSorted(std::list<int> const &);