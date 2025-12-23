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
	if (readData() == false && printExchange(file) == false)
		exit(EXIT_FAILURE);
}

bool	BitcoinExchange::openFile(std::string const & file, std::ifstream & input)
{
	if (file.empty() == true)
	{
		std::cout << "Error: could not open file." << std::endl;
		return (false);
	}

	input.open(file.c_str());

	if (input.is_open() == false)
	{
		std::cout << "Error: could not open file." << std::endl;
		return (false);
	}

	return (true);
}

static bool	isLeapYear(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

static bool	isValidDate(std::string const & date)
{
	int	daysInMonth[] = DATE;

	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return (false);

	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!isdigit(date[i]))
			return (false);
	}

	int year = atoi(date.substr(0, 4).c_str());
	int month = atoi(date.substr(5, 2).c_str());
	int day = atoi(date.substr(8, 2).c_str());

	if (year < 0 || month < 1 || month > 12 || day < 1)
		return (false);

	int maxDay = daysInMonth[month - 1];
	if (month == 2 && isLeapYear(year))
		maxDay = 29;

	if (day > maxDay)
		return (false);

	return (true);
}

static bool	isValidValue(std::string const & value)
{
	size_t	i = 0;
	bool	hasDot = false;

	if (value.empty())
		return (false);

	if (value[0] == '-' || value[0] == '+')
		i++;

	if (i >= value.length())
		return (false);

	while (i < value.length())
	{
		if (value[i] == '.')
		{
			if (hasDot)
				return (false);
			if (i == 0 || i == value.length() - 1)
				return (false);
			hasDot = true;
		}
		else if (!isdigit(value[i]))
			return (false);
		i++;
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
				std::cout << "Error: invalid header format." << std::endl;
				return (false);
			}
			continue ;
		}

		size_t commaPos = buffer.find(',');
		if (commaPos == std::string::npos)
		{
			std::cout << "Error: bad input => " << buffer << std::endl;
			continue ;
		}

		std::string date = buffer.substr(0, commaPos);
		std::string value = buffer.substr(commaPos + 1);

		if (!isValidDate(date))
		{
			std::cout << "Error: invalid date => " << date << std::endl;
			continue;
		}

		if (!isValidValue(value))
		{
			std::cout << "Error: invalid value => " << value << std::endl;
			continue;
		}
		else if (atoi(value.c_str()) > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}
		else if (atof(value.c_str()) < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}

		if (_data.find(date) != _data.end())
		{
			std::cout << "Error: duplicate date entry => " << date << std::endl;
			
			continue;
		}

		_data[date] = value;
		line++;
	}

	input.close();
	return (true);
}

static std::string	trim(std::string const & str)
{
	size_t	start = 0;
	size_t	end = str.length();

	while (start < end && std::isspace(str[start]))
		start++;

	while (end > start && std::isspace(str[end - 1]))
		end--;

	return (str.substr(start, end - start));
}

bool	BitcoinExchange::printExchange(std::string const & inputFile)
{
	std::ifstream	input;
	std::string		buffer;
	bool			firstLine = true;

	if (openFile(inputFile, input) == false)
		return (false);

	while (std::getline(input, buffer))
	{
		if (firstLine)
		{
			firstLine = false;
			continue;
		}

		size_t pipePos = buffer.find('|');
		if (pipePos == std::string::npos)
		{
			std::cout << "Error: bad input => " << buffer << std::endl;
			continue;
		}

		std::string date = trim(buffer.substr(0, pipePos));
		std::string valueStr = trim(buffer.substr(pipePos + 1));

		if (!isValidDate(date))
		{
			std::cout << "Error: bad input => " << buffer << std::endl;
			continue;
		}

		if (!isValidValue(valueStr))
		{
			std::cout << "Error: bad input => " << buffer << std::endl;
			continue;
		}

		double value = atof(valueStr.c_str());

		if (value < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}

		if (value > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}

		std::map<std::string, std::string>::iterator it = _data.lower_bound(date);
		
		if (it == _data.end() || it->first != date)
		{
			if (it == _data.begin())
			{
				std::cout << "Error: date too early." << std::endl;
				continue;
			}
			--it;
		}

		double exchangeRate = atof(it->second.c_str());
		double result = value * exchangeRate;

		std::cout << date << " => " << valueStr << " = " << result << std::endl;
	}

	input.close();
	return (true);
}