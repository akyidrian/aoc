#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace fileio
{
    // TODO: Probably should be using std::filesystem rather than std::string
    inline std::vector<std::string> readFile(const std::string& filePath)
    {
	std::ifstream inputFile(filePath);
        if(!inputFile.is_open())
        {
	    std::cerr << "Error opening input file: " << filePath << '\n';
            return {};
        }
	std::vector<std::string> input;
	std::string line;
        while(std::getline(inputFile, line))
        {
            input.push_back(line);
        }
        return input;
    }
};
