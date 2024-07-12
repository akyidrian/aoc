#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <regex>
#include <unordered_map>
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
using std::unordered_map;

using SchematicPosition = pair<size_t, size_t>;

namespace {
    struct SchematicNumber
    {
        unsigned int m_number = 0;
        size_t m_row = 0;
        size_t m_colBegin = 0;
        size_t m_colEnd = 0;
    };

    struct SchematicSymbol
    {
        char m_symbols = '.';  // '.' does not count as symbols
        vector<SchematicNumber> m_neighbours;
    };

    struct PairHash
    {
        size_t operator()(const SchematicPosition& p) const
        {
            return p.first << 16 & p.second;
        }
    };

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

    vector<SchematicPosition> constructNeighbours(size_t rrr, size_t ccc, size_t rows, size_t cols)
    {
        vector<SchematicPosition> neighbours;
        if(rrr > 0) // top
        {
            neighbours.push_back({rrr-1, ccc});
        }
        if(rrr < (rows-1)) // bottom
        {
            neighbours.push_back({rrr+1, ccc});
        }
        if(ccc > 0) // left
        {
            neighbours.push_back({rrr, ccc-1});
        }
        if(ccc < (cols-1)) // right
        {
            neighbours.push_back({rrr, ccc+1});
        }
        if(rrr > 0 && ccc > 0) // top-left
        {
            neighbours.push_back({rrr-1, ccc-1}); 
        }
        if(rrr < (rows-1) && ccc < (cols-1)) // bottom-right
        {
            neighbours.push_back({rrr+1, ccc+1});
        }
        if(rrr > 0 && ccc < (cols-1)) // top-right
        {
            neighbours.push_back({rrr-1, ccc+1});
        }
        if(rrr < (rows-1) && ccc > 0) // bottom-left
        {
            neighbours.push_back({rrr+1, ccc-1});
        }
        return neighbours;
    }

    bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    SchematicNumber constructNumber(const vector<string>& schematic, const size_t row, const size_t col, set<SchematicPosition>& alreadyVisited)
    {
        const auto line = schematic[row];
        auto first = line.begin() + static_cast<unsigned int>(col);
        auto last = line.begin() + static_cast<unsigned int>(col);
        while(std::prev(first) >= line.begin() && isDigit(*std::prev(first)))
        {
            first--; 
        }
        while(std::next(last) < line.end() && isDigit(*std::next(last)))
        {
            last++;
        }
        const auto colFirst = static_cast<size_t>(first-line.begin());
        const auto colLast = static_cast<size_t>(last-line.begin());
        for(auto ccc = colFirst; ccc <= colLast; ++ccc)
        {
            alreadyVisited.insert({row, ccc});
        }
        const auto number = static_cast<unsigned int>(stoul(line.substr(colFirst, colLast-colFirst+1)));
        return {number, row, colFirst, colLast};
    }

    vector<unsigned int> parseEngineSchematic(const vector<string>& schematic)
    {
        auto isDot = [](char c){ return c == '.'; };
        auto isSymbol = [&isDot](char c){ return !isDot(c) && !isDigit(c); };
        auto isStar = [](char c){ return c == '*'; };
        unordered_map<pair<size_t /*row*/, size_t /*col*/>, SchematicSymbol, PairHash> symbols;
        vector<unsigned int> partNumbers;
        const auto rowCount = schematic.size(); 
        const auto colCount = schematic[0].length();
        for(auto row = 0uz; row < rowCount; ++row)
        {
            const auto line = schematic[row];

            // Looping through each symbol...
            for(auto it = line.begin(); (it = find_if(it, line.end(), isSymbol)) != line.end(); ++it)
            {
                const auto col = static_cast<size_t>(it - line.begin());
                const auto neighbours = constructNeighbours(row, col, rowCount, colCount);
                set<SchematicPosition> alreadyVisited;
                for(const auto& [rrr, ccc] : neighbours)
                {
                    if(!alreadyVisited.contains({rrr, ccc}) && isDigit(schematic[rrr][ccc]))
                    {
                        const auto number = constructNumber(schematic, rrr, ccc, alreadyVisited);
                        symbols[{row, col}].m_neighbours.push_back(number);
                        partNumbers.push_back(number.m_number);
                    }
                }
            }
        }

        return partNumbers;
    }
};

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

    const auto partNumbers = parseEngineSchematic(schematic);
    const auto sum = accumulate(partNumbers.begin(), partNumbers.end(), 0);
    cout << "Sum of part numbers: " << sum << '\n';

    return 0;
}
