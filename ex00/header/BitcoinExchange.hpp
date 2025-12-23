#pragma once

#include <iostream>
#include <map>
#include <fstream>

# define DATE {{31}, {28}, {31}, {30}, {31}, {30}, {31}, {31}, {30}, {31}, {30}, {31}}

class BitcoinExchange
{
	public:
											BitcoinExchange(std::string const &);
											BitcoinExchange(BitcoinExchange const &);

		BitcoinExchange &					operator=(BitcoinExchange const &);

											~BitcoinExchange();

	private:
											BitcoinExchange();
		std::map<std::string, std::string>	_data;

		bool								readData();
		bool								openFile(std::string const &, std::ifstream &);

};