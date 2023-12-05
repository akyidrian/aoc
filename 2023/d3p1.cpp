#include <fstream>
#include <iostream>
#include <numeric>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <regex>
#include <vector>

using std::accumulate;
using std::cerr;
using std::cout;
using std::ifstream;
using std::ostream;
using std::pair;
using std::set;
using std::smatch;
using std::string;
using std::stringstream;
using std::string_view;
using std::regex;
using std::sregex_iterator;
using std::vector;

vector<string> readEngineSchematic(const string& filePath)
{
    ifstream inputFile(filePath);
    if(!inputFile.is_open())
    {
        cerr << "Error opening input file: " << filePath << '\n';
        return {};
    }
    vector<string> input;
    string line;
    while(getline(inputFile, line))
    {
        input.push_back(line);
    }
    return input;
}

struct SchematicNumber
{
    unsigned int number;
    bool isPart = false;
    unsigned int row;
    unsigned int colBegin;
    unsigned int colEnd;
};

vector<unsigned int> parseEngineSchematic(const vector<string>& schematic)
{
    auto isDot = [](char c){ return c == '.'; };
    auto notDot = [&isDot](char c){ return !isDot(c); };
    auto isDigit = [](char c) { return c >= '0' && c <= '9'; };
    auto isSymbol = [&notDot, &isDigit](char c) { return notDot(c) && !isDigit(c); };
    vector<SchematicNumber> numbers;
    std::set<std::pair<unsigned int /*row*/, unsigned int /*col*/>> symbolPositions;
    for(auto row = 0u; row < schematic.size(); ++row)
    {
        const auto line = schematic[row];

        // Looping through each "non-dot" char...
        for(auto it = line.begin(); (it = find_if(it, line.end(), notDot)) != line.end();)
        {
            const auto colBegin = static_cast<unsigned int>(it - line.begin());
            if(isSymbol(*it))
            {
                symbolPositions.insert({row, colBegin});  // symbols are just one char
                ++it;
            }
            else if(isDigit(*it))
            {
                while(++it != line.end() && isDigit(*it))
                {
                    // Do nothing...
                }
                const auto colEnd = static_cast<unsigned int>(it - line.begin());
                const auto number = static_cast<unsigned int>(stoul(line.substr(colBegin, colEnd - colBegin)));
                numbers.push_back({number, false, row, colBegin, colEnd-1});  // colEnd-1 for the last char pos
            }
        }
    }

    // TODO: Perhaps not the cleanest code but it works...
    //       We may be double checking things under specific circumstances.
    auto isPart = [&schematic, &symbolPositions](const SchematicNumber& sn) -> bool
    {
        const auto top = sn.row == 0 ? 0 : sn.row - 1;
        const auto bottom = sn.row == (schematic.size() - 1) ? schematic.size() - 1 : sn.row + 1;
        const auto left = sn.colBegin == 0 ? 0 : sn.colBegin - 1;
        const auto right = sn.colEnd == schematic[0].length() - 1 ? schematic[0].length() - 1 : sn.colEnd + 1;
        for(auto ccc = left; ccc <= right; ++ccc)
        {
            if(symbolPositions.contains({top, ccc}))
            {
                return true;
            }
        }
        if(symbolPositions.contains({sn.row, left}) || symbolPositions.contains({sn.row, right}))
        {
            return true;
        }
        for(auto ccc = left; ccc <= right; ++ccc)
        {
            if(symbolPositions.contains({bottom, ccc}))
            {
                return true;
            }
        }
        return false;
    };
    vector<unsigned int> parts;
    for(const auto& n : numbers)
    {
        if(isPart(n))
        {
            parts.push_back(n.number);
        }
    }
    return parts;
}

int main(int argc, char** argv)
{
    vector<string> example{
        "467..114..",
        "...*......",
        "..35..633.",
        "......#...",
        "617*......",
        ".....+.58.",
        "..592.....",
        "......755.",
        "...$.*....",
        ".664.598.."
    };
    vector<string>& schematic = example;

    const auto useExample = argc == 1;
    if(!useExample)
    {
        schematic = readEngineSchematic(argv[1]);
    }

    auto partNumbers = parseEngineSchematic(schematic);
    auto sum = accumulate(partNumbers.begin(), partNumbers.end(), 0);
    cout << "Sum of part numbers: " << sum << '\n';

    return 0;
}
