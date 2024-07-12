#include "fileio.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using std::accumulate;
using std::cout;
using std::find_if;
using std::ifstream;
using std::string;
using std::vector;

unsigned int findCalibrationValue(const string& line)
{
    auto isDigit = [](unsigned char c){ return c >= '0' && c <= '9'; };
    const auto firstDigit = static_cast<unsigned int>(*find_if(line.begin(), line.end(), isDigit) - '0');
    const auto secondDigit = static_cast<unsigned int>(*find_if(line.rbegin(), line.rend(), isDigit) - '0');
    return firstDigit * 10 + secondDigit;
}

int main(int argc, char** argv)
{
    vector<string> example{
        "1abc2",
        "pqr3stu8vwx",
        "a1b2c3d4e5f",
        "treb7uchet"
    };
    vector<string>& doc = example;

    const auto useExample = argc == 1;
    if(!useExample)
    {
        doc = fileio::readFile(argv[1]); // read calibration doc
    }

    vector<unsigned int> values;
    values.reserve(doc.size());
    for(const auto& l : doc)
    {
        values.push_back(findCalibrationValue(l));
    }
    auto sum = accumulate(values.begin(), values.end(), 0);
    cout << "Sum of calibration values = " << sum << '\n';

    return 0;
}
