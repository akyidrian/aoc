#include "helper.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

using std::accumulate;
using std::cout;
using std::find_if;
using std::function;
using std::ifstream;
using std::string;
using std::unordered_map;
using std::vector;

unsigned int findCalibrationValuePart1(const string& line)
{
    auto isDigit = [](unsigned char c){ return c >= '0' && c <= '9'; };
    const auto firstDigit = static_cast<unsigned int>(*find_if(line.begin(), line.end(), isDigit) - '0');
    const auto secondDigit = static_cast<unsigned int>(*find_if(line.rbegin(), line.rend(), isDigit) - '0');
    return firstDigit * 10 + secondDigit;
}

unsigned int findCalibrationValuePart2(const string& line)
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

[[nodiscard]] unsigned int run(const vector<string>& doc, const function<unsigned int(const string&)>& findCalibrationValueFunc)
{
    vector<unsigned long> values;
    values.reserve(doc.size());
    for(const auto& l : doc)
    {
        values.push_back(findCalibrationValueFunc(l));
    }
    return accumulate(values.begin(), values.end(), 0u);
}

int main(int argc, char** argv)
{
    using namespace helper;

    const auto [part, filePath] = parseMainArgs(argc, argv);
    switch(part)
    {
        case Part::One:
        {
            vector<string> example{
                "1abc2",
                "pqr3stu8vwx",
                "a1b2c3d4e5f",
                "treb7uchet"
            };
            constexpr auto exampleAnswer = 142u;
            constexpr auto inputAnswer = 56108u;
            vector<string> doc = example;
            auto expected = exampleAnswer;
            if(!filePath.empty())
            {
                doc = readFile(filePath);
                expected = inputAnswer;
            }
            const auto answer = run(doc, findCalibrationValuePart1);
            cout << "Sum of calibration values = " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
        case Part::Two:
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
            constexpr auto exampleAnswer = 281u;
            constexpr auto inputAnswer = 55652u;
            vector<string> doc = example;
            auto expected = exampleAnswer;
            if(!filePath.empty())
            {
                doc = readFile(argv[2]);
                expected = inputAnswer;
            }
            const auto answer = run(doc, findCalibrationValuePart2);
            cout << "Sum of calibration values = " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
    }

    return 0;
}
