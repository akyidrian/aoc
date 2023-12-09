#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
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

    // Works in the same way as python's split function
    inline std::vector<std::string> split(const std::string& str, const char separator)
    {
        std::stringstream ss(str);
        std::vector<std::string> splitStrs;
        for(std::string temp; std::getline(ss, temp, separator);)
        {
            splitStrs.push_back(temp);
        }
        if(str.back() == separator)
        {
            splitStrs.push_back("");
        }
        return splitStrs;
    }

    // Works in the same way as python's strip function
    inline std::string strip(const std::string& str)
    {
        auto beginIt = str.begin();
        auto endIt = str.end();
        while(beginIt != str.end() && *beginIt == ' ')
        {
            beginIt++;
        }
        if(beginIt == str.end())
        {
            return "";
        }
        while(*prev(endIt) == ' ')
        {
            endIt--;
        }
        const auto begin = static_cast<std::string::size_type>(beginIt-str.begin());
        const auto length = static_cast<std::string::size_type>(endIt-beginIt);
        return str.substr(begin, length);
    }
};
