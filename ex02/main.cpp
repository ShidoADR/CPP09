#include "header/PmergeMe.hpp"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error : Not enough arguments" << std::endl;
		return (1);
	}

	std::string input;

	for (int i = 1; i < argc; i++)
	{
		input += argv[i];
		
		if (i < argc - 1)
			input += " ";
	}

	try
	{
		PmergeMe pmergeMe(input);

		std::cout << pmergeMe << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		
		return (1);
	}

	return 0;
}