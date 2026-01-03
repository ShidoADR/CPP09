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

const std::vector<int> & PmergeMe::getData() const
{
	return (_data);
}

const std::list<int> &	PmergeMe::getListData() const
{
	return (_listData);
}

const std::vector<int> &	PmergeMe::getSortedData() const
{
	return (_sortedData);
}

const std::list<int> &	PmergeMe::getSortedListData() const
{
	return (_sortedListData);
}

double	PmergeMe::getTimeForList() const
{
	return (_timeForList);
}

double	PmergeMe::getTimeForVector() const
{
	return (_timeForVector);
}

std::ostream &	operator<<(std::ostream & output, PmergeMe const & instance)
{
	std::vector<int>	data = instance.getData();
	std::vector<int>	sortedData = instance.getSortedData();
	std::list<int>		sortedListData = instance.getSortedListData();
	double 			timeForVector = instance.getTimeForVector();
	double 			timeForList = instance.getTimeForList();


	if (isSorted(sortedData) == false || isSorted(sortedListData) == false)
		output << "Error: Data is not sorted correctly." << std::endl;
	else
	{
		output << "Before: ";
		for (size_t i = 0; i < data.size(); i++)
		{
			output << data[i];
			if (i < data.size() - 1)
				output << " ";
		}

		output << std::endl << "After:  ";
		for (size_t i = 0; i < sortedData.size(); i++)
		{
			output << sortedData[i];
			if (i < sortedData.size() - 1)
				output << " ";
		}
		output << std::endl;

		output << "Time to process a range of " << data.size() << " elements with std::vector : " << std::fixed << std::setprecision(6) << timeForVector << " s" << std::endl;
		output << "Time to process a range of " << data.size() << " elements with std::list   : " << std::fixed << std::setprecision(6) << timeForList << " s" << std::endl;
	}

	return (output);
}

PmergeMe::PmergeMe(std::string const & input)
{
	std::istringstream	ss(input);
	std::string			token;

	while (ss >> token)
	{
		if (isNumber(token) == false)
			throw std::invalid_argument("Error: " + token + " is not a valid number");

		long	num = std::strtoll(token.c_str(), NULL, 10);

		if (num > INT_MAX)
			throw std::out_of_range("Error: " + token + " is out of range");

		if (std::find(_data.begin(), _data.end(), static_cast<int>(num)) != _data.end())
			throw std::invalid_argument("Error: " + token + " is a duplicate number");

		_data.push_back(static_cast<int>(num));
		_listData.push_back(static_cast<int>(num));
	}

	if (_data.empty() == true)
		throw std::invalid_argument("Error: no valid numbers provided");

	std::clock_t		start = std::clock();
	_sortedData = mergeInsertVector(_data);
	std::clock_t		end = std::clock();
	_timeForVector = static_cast<double>(end - start) / CLOCKS_PER_SEC;

	start = std::clock();	
	_sortedListData = mergeInsertList(_listData);
	end = std::clock();
	_timeForList = static_cast<double>(end - start) / CLOCKS_PER_SEC;

}

void	PmergeMe::makePair(std::vector<int> const & data, std::vector<int> & main, std::vector<int> & pend)
{
	size_t				size = data.size() % 2 == 0 ? data.size() : data.size() - 1;
	std::vector<int>	temp;

	for (size_t i = 0; i < size; i += 2)
	{
		if (data[i] > data[i + 1])
		{
			temp.push_back(data[i]);
			pend.push_back(data[i + 1]);
		}
		else
		{
			temp.push_back(data[i + 1]);
			pend.push_back(data[i]);
		}
	}

	main = temp;
}

std::vector<int> PmergeMe::jacobsthalSequence(size_t n)
{
	std::vector<int>	sequence;
	size_t				jacobsthal = 1;
	int					prevJacobsthal = 0;

	while (jacobsthal <= n)
	{	
		for (int i = jacobsthal; i > prevJacobsthal; i--)
			sequence.push_back(i);
	
		int temp = jacobsthal;
		jacobsthal = jacobsthal + (2 * prevJacobsthal);
		prevJacobsthal = temp;
	}

	if (sequence.size() < n)
	{
		for (int i = n; i > prevJacobsthal; i--)
			sequence.push_back(i);
	}
	return (sequence);
}

void	PmergeMe::updatePend(std::vector<int> & pend, std::vector<int> & main, std::vector<int> & main_pair)
{
	if (pend.empty())
		return ;

	std::vector<int>	pend_ordered;

	for (size_t i = 0; i < main.size(); ++i)
	{
		std::vector<int>::iterator it = std::find(main_pair.begin(), main_pair.end(), main[i]);
		if (it != main_pair.end())
		{
			size_t idx = std::distance(main_pair.begin(), it);
			if (idx < pend.size())
				pend_ordered.push_back(pend[idx]);
		}
	}

	pend = pend_ordered;
}

std::vector<int> PmergeMe::mergeInsertVector(std::vector<int> const & data)
{
	if (data.size() == 1)
		return (data);

	std::vector<int>	main_pair;
	std::vector<int>	main = data;
	std::vector<int> 	pend;
	std::vector<int>	jacobsthal;
	int					left = data.size() % 2 != 0 ? data.back() : -1;
	
	makePair(main, main, pend);

	main_pair = main;

	if (left != -1)
		pend.push_back(left);
	jacobsthal = jacobsthalSequence(pend.size());

	if (main.size() > 1)
		main = mergeInsertVector(main);
	
	if (left != -1)
		pend.pop_back();

	updatePend(pend, main, main_pair);

	if (left != -1)
		pend.push_back(left);
	
	main_pair = main;

	main.insert(main.begin(), pend[0]);

	int	prevInserted = pend[0];
	int	idxOffset = 0;

	for (size_t i = 1; i < jacobsthal.size(); i++)
	{
		std::vector<int>::iterator	pos;
		if (left != -1 && static_cast<size_t>(jacobsthal[i]) == pend.size())
		{
			prevInserted = pend[jacobsthal[i] - 1];
			idxOffset += main_pair[jacobsthal[i] - 2] > prevInserted ? 1 : 0;
			pos = std::lower_bound(main.begin(), main.end(), prevInserted);
		}
		else
		{
			std::vector<int>::iterator	end = main.begin();
			int							idx = main_pair[jacobsthal[i] - 1] > prevInserted ? jacobsthal[i] + idxOffset + 1 : jacobsthal[i] + idxOffset;

			std::advance(end, idx);

			prevInserted = pend[jacobsthal[i] - 1];

			idxOffset += main_pair[jacobsthal[i] - 1] > prevInserted ? 1 : 0;

			pos = std::lower_bound(main.begin(), end, prevInserted);
		}
		
		
		main.insert(pos, prevInserted);
	}

	return (main);
}

std::list<int>	PmergeMe::jacobsthalSequenceList(size_t n)
{
	std::list<int>		sequence;
	size_t				jacobsthal = 1;
	int					prevJacobsthal = 0;

	while (jacobsthal <= n)
	{	
		for (int i = jacobsthal; i > prevJacobsthal; i--)
			sequence.push_back(i);
	
		int temp = jacobsthal;
		jacobsthal = jacobsthal + (2 * prevJacobsthal);
		prevJacobsthal = temp;
	}

	if (sequence.size() < n)
	{
		for (int i = n; i > prevJacobsthal; i--)
			sequence.push_back(i);
	}
	return (sequence);
}

void	PmergeMe::makePair(std::list<int> const & data, std::list<int> & main, std::list<int> & pend)
{
	size_t				size = data.size() % 2 == 0 ? data.size() : data.size() - 1;
	std::list<int>		temp;

	for (std::list<int>::const_iterator it = data.begin(); size > 0; )
	{
		int first = *it;
		++it;
		int second = *it;
		++it;

		if (first > second)
		{
			temp.push_back(first);
			pend.push_back(second);
		}
		else
		{
			temp.push_back(second);
			pend.push_back(first);
		}
		size -= 2;
	}

	main = temp;
}

void	PmergeMe::updatePend(std::list<int> & pend, std::list<int> & main, std::list<int> & main_pair)
{
	if (pend.empty())
		return ;

	std::list<int>		pend_ordered;

	for (std::list<int>::iterator it = main.begin(); it != main.end(); ++it)
	{
		std::list<int>::iterator it_pair = std::find(main_pair.begin(), main_pair.end(), *it);
		if (it_pair != main_pair.end())
		{
			size_t idx = std::distance(main_pair.begin(), it_pair);
			std::list<int>::iterator it_pend = pend.begin();
			std::advance(it_pend, idx);
			if (it_pend != pend.end())
				pend_ordered.push_back(*it_pend);
		}
	}

	pend = pend_ordered;
}


std::list<int>	PmergeMe::mergeInsertList(std::list<int> const & data)
{
	if (data.size() == 1)
		return (data);

	std::list<int>		main_pair;
	std::list<int>		main = data;
	std::list<int> 		pend;
	std::list<int>		jacobsthal;
	int					left = data.size() % 2 != 0 ? data.back() : -1;
	
	makePair(main, main, pend);

	main_pair = main;

	if (left != -1)
		pend.push_back(left);

	jacobsthal = jacobsthalSequenceList(pend.size());

	if (main.size() > 1)
		main = mergeInsertList(main);
	
	if (left != -1)
		pend.pop_back();

	updatePend(pend, main, main_pair);

	if (left != -1)
		pend.push_back(left);

	main_pair = main;

	main.push_front(pend.front());

	int	prevInserted = pend.front();
	int	idxOffset = 0;

	for (std::list<int>::iterator it = jacobsthal.begin(); it != jacobsthal.end(); ++it)
	{
		if (*it == 1)
			continue ;

		std::list<int>::iterator	mainBound = main_pair.begin();
		std::advance(mainBound, *it - 1);

		std::list<int>::iterator	pendIt = pend.begin();
		std::advance(pendIt, *it - 1);

		if (left != -1 && static_cast<size_t>(*it) == pend.size())
		{
			mainBound--;
			prevInserted = *pendIt;

			idxOffset += *mainBound > prevInserted ? 1 : 0;
	
			std::list<int>::iterator	pos = std::lower_bound(main.begin(), main.end(), prevInserted);

			main.insert(pos, prevInserted);

			continue ;
		}

		std::list<int>::iterator	end = main.begin();

		int							idx = *mainBound > prevInserted ? *it + idxOffset + 1 : *it + idxOffset;

		std::advance(end, idx);

		prevInserted = *pendIt;

		idxOffset += *mainBound > prevInserted ? 1 : 0;

		std::list<int>::iterator	pos = std::lower_bound(main.begin(), end, prevInserted);
		
		main.insert(pos, prevInserted);
	}

	return (main);
}

bool	isSorted(std::vector<int> const & data)
{
	for (size_t i = 1; i < data.size(); i++)
	{
		if (data[i - 1] > data[i])
			return (false);
	}
	return (true);
}

bool	isSorted(std::list<int> const & data)
{
	if (data.empty() == true)
		return (true);

	std::list<int>::const_iterator	prev = data.begin();
	std::list<int>::const_iterator	it = prev;
	++it;

	for (; it != data.end(); ++it, ++prev)
	{
		if (*prev > *it)
			return (false);
	}
	return (true);
}