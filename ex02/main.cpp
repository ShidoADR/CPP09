#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <PmergeMe.hpp>

static bool isPositiveInteger(const std::string &s, int &out)
{
    if (s.empty())
        return false;
    size_t i = 0;
    if (s[0] == '+')
    {
        if (s.size() == 1)
            return false;
        i = 1;
    }
    for (; i < s.size(); ++i)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }
    char *endptr = 0;
    long val = std::strtol(s.c_str(), &endptr, 10);
    if (*endptr != '\0')
        return false;
    if (val <= 0 || val > INT_MAX)
        return false;
    out = static_cast<int>(val);
    return true;
}

static void printSequence(const std::vector<int> &seq)
{
    for (size_t i = 0; i < seq.size(); ++i)
    {
        std::cout << seq[i];
        if (i + 1 < seq.size())
            std::cout << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    std::vector<int> inputVec;
    std::deque<int> inputDeq;
    inputVec.reserve(argc - 1);

    for (int i = 1; i < argc; ++i)
    {
        int val;
        if (!isPositiveInteger(std::string(argv[i]), val))
        {
            std::cerr << "Error" << std::endl;
            return 1;
        }
        inputVec.push_back(val);
        inputDeq.push_back(val);
    }

    std::cout << "Before: ";
    printSequence(inputVec);

    // Time vector-based Ford-Johnson
    clock_t vStart = std::clock();
    std::vector<int> sortedVec = PmergeMe::sortVector(inputVec);
    clock_t vEnd = std::clock();
    double vUs = (double)(vEnd - vStart) * 1e6 / (double)CLOCKS_PER_SEC;

    // Time deque-based Ford-Johnson
    clock_t dStart = std::clock();
    std::deque<int> sortedDeq = PmergeMe::sortDeque(inputDeq);
    clock_t dEnd = std::clock();
    double dUs = (double)(dEnd - dStart) * 1e6 / (double)CLOCKS_PER_SEC;

    // Output sorted sequence (use vector result)
    std::cout << "After: ";
    printSequence(sortedVec);

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << inputVec.size()
              << " elements with std::vector : " << vUs << " us" << std::endl;
    std::cout << "Time to process a range of " << inputDeq.size()
              << " elements with std::deque : " << dUs << " us" << std::endl;

    return 0;
}