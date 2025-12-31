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

std::ostream &	operator<<(std::ostream & output, PmergeMe const & instance)
{
	const std::vector<int> data = instance.getData();

	for (size_t i = 0; i < data.size(); i++)
	{
		output << data[i];
		if (i < data.size() - 1)
			output << " ";
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
		_dequeData.push_back(static_cast<int>(num));
	}

	if (_data.empty() == true)
		throw std::invalid_argument("Error: no valid numbers provided");

	std::vector<int>	merged = mergeInsertVector(_data);

	for (int i = 0; i < static_cast<int>(merged.size()); i++)
		std::cout << " " << merged[i];
	std::cout << std::endl;
}

void	PmergeMe::makePair(std::vector<int> const & data, std::vector<int> & main, std::vector<int> & pend)
{
	size_t				size = data.size();
	std::vector<int>	temp;

	if (data.size() % 2 != 0)
		size -= 1;

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
	int					prevJacobsthal = 0;
	size_t				jacobsthal = 1;

	while (jacobsthal < n)
	{	
		for (int i = jacobsthal; i > prevJacobsthal; i--)
			sequence.push_back(i);
	
		int temp = jacobsthal;
		jacobsthal = jacobsthal + (2 * prevJacobsthal);
		prevJacobsthal = temp;
	}

	if (sequence.size() < n)
	{
		int	last = sequence.empty() ? 0 : sequence.back();
		for (int i = n; i > last; i--)
			sequence.push_back(i);
	}
	return (sequence);
}

void	PmergeMe::updatePend(std::vector<int> & pend, std::vector<int> & main, std::vector<int> & main_pair)
{
	if (pend.empty())
		return;

	std::vector<int>	pend_ordered;
        pend_ordered.reserve(pend.size());

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

	// std::cout << "Main: ";
	// for (size_t i = 0; i < main.size(); i++)
	// 	std::cout << main[i] << " ";
	// std::cout << std::endl;

	// std::cout << "Pend: ";
	// for (size_t i = 0; i < pend.size(); i++)
	// 	std::cout << pend[i] << " ";
	// std::cout << std::endl;

	main_pair = main;
	jacobsthal = jacobsthalSequence(pend.size());

	if (main.size() > 1)
		main = mergeInsertVector(main);
	
	updatePend(pend, main, main_pair);

	main_pair = main;

	main.insert(main.begin(), pend[0]);

	for (size_t i = 1; i < jacobsthal.size(); i++)
	{
		std::vector<int>::iterator	end = std::find(main.begin(), main.end(), main_pair[jacobsthal[i] - 1]);
		// std::cout << "aaaa " << *end << " " << pend[jacobsthal[i] - 1] <<  std::endl; 
		std::vector<int>::iterator	pos = std::lower_bound(main.begin(), end, pend[jacobsthal[i] - 1]);
		
		main.insert(pos, pend[jacobsthal[i] - 1]);
	}
	
	if (left != -1)
	{
		std::vector<int>::iterator	pos = std::lower_bound(main.begin(), main.end(), left);
		main.insert(pos, left);
	}

	std::cout << "Main After: ";
	for (size_t i = 0; i < main.size(); i++)
		std::cout << main[i] << " ";
	std::cout << std::endl;

	std::cout << "Pend After: ";
	for (size_t i = 0; i < pend.size(); i++)
		std::cout << pend[i] << " ";
	std::cout << std::endl;


		std::cout << "Jacobsthal: ";
	for (size_t i = 0; i < jacobsthal.size(); i++)
		std::cout << jacobsthal[i] << " ";
	std::cout << std::endl;
	return (main);
}