#include "helper.h"

#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::function;
using std::pair;
using std::set;
using std::string;
using std::vector;
using std::unordered_map;

struct SchematicSymbol;
struct PairHash;
using SchematicPosition = pair<size_t, size_t>;
using SchematicSymbolMap = unordered_map<SchematicPosition, SchematicSymbol, PairHash>;

struct SchematicSymbol
{
    char m_symbols = '.';  // '.' does not count as symbols
    vector<unsigned int> m_neighbours;
};

// Using cantor pairing...
struct PairHash
{
    size_t operator()(const SchematicPosition& p) const
    {
        const auto [a, b] = p;
        return (a + b) * (a + b + 1) / 2 + a;
    }
};

vector<SchematicPosition> constructNeighbours(size_t rrr, size_t ccc, size_t rows, size_t cols)
{
    const auto left = ccc - 1;
    const auto right = ccc + 1;
    const auto top = rrr - 1;
    const auto bottom = rrr + 1;
    vector<SchematicPosition> neighbours;
    if(rrr > 0) // top
    {
        neighbours.push_back({top, ccc});
    }
    if(rrr < (rows-1)) // bottom
    {
        neighbours.push_back({bottom, ccc});
    }
    if(ccc > 0) // left
    {
        neighbours.push_back({rrr, left});
    }
    if(ccc < (cols-1)) // right
    {
        neighbours.push_back({rrr, right});
    }
    if(rrr > 0 && ccc > 0) // top-left
    {
        neighbours.push_back({top, left});
    }
    if(rrr < (rows-1) && ccc < (cols-1)) // bottom-right
    {
        neighbours.push_back({bottom, right});
    }
    if(rrr > 0 && ccc < (cols-1)) // top-right
    {
        neighbours.push_back({top, right});
    }
    if(rrr < (rows-1) && ccc > 0) // bottom-left
    {
        neighbours.push_back({bottom, left});
    }
    return neighbours;
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

unsigned int constructNumber(const vector<string>& schematic, const size_t row, const size_t col, set<SchematicPosition>& alreadyVisited)
{
    const auto line = schematic[row];
    auto first = line.begin() + static_cast<std::string::difference_type>(col);
    auto last = line.begin() + static_cast<std::string::difference_type>(col);
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
    return static_cast<unsigned int>(stoul(line.substr(colFirst, colLast-colFirst+1)));
}

SchematicSymbolMap parseEngineSchematic(const vector<string>& schematic, function<bool(char)> isSymbol)
{
    SchematicSymbolMap symbols;
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
                }
            }
        }
    }

    return symbols;
}

[[nodiscard]] unsigned int runPart1(const vector<string>& schematic)
{
    auto isDot = [](char c){ return c == '.'; };
    auto isSymbol = [&isDot](char c){ return !isDot(c) && !isDigit(c); };
    const auto& symbolMap = parseEngineSchematic(schematic, isSymbol);
    auto sumOfPartNumbers = 0u;
    for(const auto& [pos, sym] : symbolMap)
    {
        for(const auto& partNumber : sym.m_neighbours)
        {
            sumOfPartNumbers += partNumber;
        }
    }
    return sumOfPartNumbers;
}

[[nodiscard]] unsigned int runPart2(const vector<string>& schematic)
{
    auto isStar = [](char c){ return c == '*'; };
    const auto& starSymbolMap = parseEngineSchematic(schematic, isStar);
    auto sumOfGearRatios = 0u;
    for(const auto& [pos, sym] : starSymbolMap)
    {
        // Gears are at least two tools connected by a star
        if(sym.m_neighbours.size() < 2)
        {
            continue;
        }
        auto gearRatio = 1u;
        for(const auto& gearNumber : sym.m_neighbours)
        {
            gearRatio *= gearNumber;
        }
        sumOfGearRatios += gearRatio;
    }
    return sumOfGearRatios;
}

int main(int argc, char** argv)
{
    using namespace helper;

    const auto [dayPart, filePath] = parseMainArgs(argc, argv);

    // Same example for both parts
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
    if(!filePath.empty())
    {
        schematic = readFile(filePath);
    }
    switch(dayPart)
    {
        case Part::One:
        {
            constexpr auto exampleAnswer = 4361u;
            constexpr auto inputAnswer = 536576u;
            const auto expected = filePath.empty() ? exampleAnswer : inputAnswer;
            const auto answer = runPart1(schematic);
            cout << "Sum of part numbers: " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
        case Part::Two:
        {
            constexpr auto exampleAnswer = 467835u;
            constexpr auto inputAnswer = 75741499u;
            const auto expected = filePath.empty() ? exampleAnswer : inputAnswer;
            const auto answer = runPart2(schematic);
            cout << "Sum of gear ratios: " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
    }

    return 0;
}
