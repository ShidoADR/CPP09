#include <BitcoinExchange.hpp>

int main(int ac, char *av[])
{
    if (ac != 2)
        std::cout << "Error: could not open file." << std::endl;
    else
    {
        std::string input = std::string(av[1]);
        BitcoinExchange	exchange(input);
    }

    return (0);
}