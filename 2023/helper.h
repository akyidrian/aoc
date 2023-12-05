#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

namespace helper
{
    enum class Part : unsigned int { One = 1u, Two, };

    inline std::pair<Part, std::string> parseMainArgs(int argc, char** argv)
    {
        if(argc > 3) // have too many args been provided?
        {
            std::cerr << "Expected at most 2 arguments in the following order: [part] [filePath]" << '\n';
            return {};
        }

        auto part = Part::One; // default to part 1 when no args provided
        if(argc >= 2) // was a part # provided?
        {
            const int partNumber = argv[1][0] - '0';
            if(partNumber < 0 || partNumber > 2)
            {
            std::cerr << "Invalid part provided. Expected '1' or '2'." << '\n';
                return {};
            }
            part = static_cast<Part>(partNumber);
        }

        const auto useFile = argc == 3 && strlen(argv[2]); // was file path provided?
        return {part, useFile ? argv[2] : ""};
    }

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
