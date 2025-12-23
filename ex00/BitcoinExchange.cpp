#include <BitcoinExchange.hpp>

BitcoinExchange::BitcoinExchange(){}

BitcoinExchange::~BitcoinExchange(){}

BitcoinExchange::BitcoinExchange(BitcoinExchange const & copy)
{
	*this = copy;
}

BitcoinExchange &	BitcoinExchange::operator=(BitcoinExchange const & copy)
{
	if (this != &copy)
		_data = copy._data;

	return (*this);
}

BitcoinExchange::BitcoinExchange(std::string const & file)
{

}

bool	BitcoinExchange::openFile(std::string const & file, std::ifstream & input)
{
	if (file.empty() == true)
	{
		std::cout << "Error: could not open file." << std::endl;
		return (false);
	}

	input.open(file);

	if (input.is_open() == false)
	{
		std::cout << "Error: could not open file." << std::endl;
		return (false);
	}

	return (true);
}

bool	BitcoinExchange::readData()
{
	std::ifstream	input;
	std::string		buffer;
	short			line = 0;

	if (openFile(std::string ("data.csv").c_str(), input) == false)
		return (false);
	
	while (std::getline (input, buffer))
	{
		if (line == 0)
		{
			if (buffer == "date,exchange_rate")
				line++;
			else
			{
				input.close();
				return (false);
			}
		}

	}

	input.close();
	return (true);
}
