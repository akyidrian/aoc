#include "fileio.h"

#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

using std::accumulate;
using std::cout;
using std::ifstream;
using std::string;
using std::unordered_map;
using std::vector;

unsigned int findCalibrationValue(const string& line)
{
    static const unordered_map<string, unsigned short> wordyDigitsMap =
        {
            {"one", 1},
            {"two", 2},
            {"three", 3},
            {"four", 4},
            {"five", 5},
            {"six", 6},
            {"seven", 7},
            {"eight", 8},
            {"nine", 9}
        };
    unsigned int firstDigit{};
    unsigned int secondDigit{};

    //
    // First find char digits...
    //
    auto firstDigitPos = line.find_first_of("123456789");
    if(firstDigitPos != string::npos)
    {
        firstDigit = static_cast<unsigned int>(line[firstDigitPos] - '0');
    }
    else {
        firstDigitPos = line.size();
    }
    auto secondDigitPos = line.find_last_of("123456789");
    if(secondDigitPos != string::npos)
    {
        secondDigit = static_cast<unsigned int>(line[secondDigitPos] - '0');
    }
    else {
        secondDigitPos = 0;
    }

    //
    // Second find wordy / spelled out digits...
    //
    for(const auto& [k,v] : wordyDigitsMap)
    {
        const auto first = line.find(k);
        if(first < firstDigitPos)  // no need to check first != npos as firstDigitPos < npos
        {
            firstDigitPos = first;
            firstDigit = v;
        }

        const auto second = line.rfind(k);
        if(second != string::npos && second > secondDigitPos)
        {
            secondDigitPos = second;
            secondDigit = v;
        }
    }
    return firstDigit * 10 + secondDigit;
}

int main(int argc, char** argv)
{
    vector<string> example{
        "two1nine",
        "eightwothree",
        "abcone2threexyz",
        "xtwone3four",
        "4nineeightseven2",
        "zoneight234",
        "7pqrstsixteen"
    };
    vector<string>& doc = example;

    const auto useExample = argc == 1;
    if(!useExample)
    {
        doc = fileio::readFile(argv[1]); // read calibration doc
    }

    vector<unsigned long> values;
    values.reserve(doc.size());
    for(const auto& l : doc)
    {
        values.push_back(findCalibrationValue(l));
    }
    auto sum = accumulate(values.begin(), values.end(), 0);
    cout << "Sum of calibration values = " << sum << '\n';

    return 0;
}
