#include "helper.h"

#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace helper;
using std::cout;
using std::smatch;
using std::string;
using std::string_view;
using std::sregex_iterator;
using std::regex;
using std::vector;

void parseAlmanac(const vector<string>& almanac)
{
    for(auto i = 0u; i < almanac.size(); ++i)
    {
        const auto& label = almanac[i];
        if(label == "")
        {
            continue;
        }
        if(label.contains("seeds:")) // colon is necessary
        {
            auto seedsStr = strip(split(label, ':')[1]);
            const auto seeds = split(seedsStr, ' ');
            // TODO: stoul seeds and store
        }
        else if(label.contains("seed-to-soil map:"))
        {
            vector<vector<unsigned int>> seedToSoil;
            for(++i; almanac[i] != ""; ++i)
            {
                auto const seedToSoilStrs = split(strip(almanac[i]), ' ');
                vector<unsigned int> temp;
                for(const auto& s : seedToSoilStrs)
                {
                    temp.push_back(static_cast<unsigned int>(stoul(s)));
                }
                seedToSoil.push_back(temp);
            }
            continue;
        }
        else if(label.contains("soil-to-fertilizer map:"))
        {
        }
        else if(label.contains("fertilizer-to-water map:"))
        {
        }
        else if(label.contains("water-to-light map:"))
        {
        }
        else if(label.contains("light-to-temperature map:"))
        {
        }
        else if(label.contains("temperature-to-humidity map:"))
        {
        }
        else if(label.contains("humidity-to-location map:"))
        {
        }
    }
    (void)almanac;
}

unsigned int runPart1(const vector<string>& almanac)
{
    parseAlmanac(almanac);
    return 0u;
}

unsigned int runPart2(const vector<string>& almanac)
{
    parseAlmanac(almanac);
    return 0u;
}

int main(int argc, char** argv)
{

    const auto [part, filePath] = parseMainArgs(argc, argv);

    // Same example for both parts
    vector<string> example{
        "seeds: 79 14 55 13",
        "",
        "seed-to-soil map:",
        "50 98 2",
        "52 50 48",
        "",
        "soil-to-fertilizer map:",
        "0 15 37",
        "37 52 2",
        "39 0 15",
        "",
        "fertilizer-to-water map:",
        "49 53 8",
        "0 11 42",
        "42 0 7",
        "57 7 4",
        "",
        "water-to-light map:",
        "88 18 7",
        "18 25 70",
        "",
        "light-to-temperature map:",
        "45 77 23",
        "81 45 19",
        "68 64 13",
        "",
        "temperature-to-humidity map:",
        "0 69 1",
        "1 0 69",
        "",
        "humidity-to-location map:",
        "60 56 37",
        "56 93 4",
    };
    vector<string>& almanac = example;
    if(!filePath.empty())
    {
        almanac = readFile(filePath);
    }
    switch(part)
    {
        case Part::One:
        {
            constexpr auto exampleAnswer = 35u;
            constexpr auto inputAnswer = 0u;
            const auto expected = filePath.empty() ? exampleAnswer : inputAnswer;
            const auto answer = runPart1(almanac);
            cout << "Lowest location number: " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
        case Part::Two:
        {
            constexpr auto exampleAnswer = 0u;
            constexpr auto inputAnswer = 0u;
            const auto expected = filePath.empty() ? exampleAnswer : inputAnswer;
            const auto answer = runPart2(almanac);
            cout << "Lowest location number: " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
    }

    return 0;
}
