#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using std::accumulate;
using std::cerr;
using std::cout;
using std::find_if;
using std::ifstream;
using std::string;
using std::vector;

vector<string> readCalibrationDoc(const string& filePath)
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

unsigned short findCalibrationValue(const string& line)
{
    auto isDigit = [](unsigned char c){ return c >= '0' && c <= '9'; };
    const auto firstDigit = find_if(line.begin(), line.end(), isDigit);
    const auto secondDigit = find_if(line.rbegin(), line.rend(), isDigit);
    return (*firstDigit - '0') * 10 + (*secondDigit - '0');
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
        doc = readCalibrationDoc(argv[1]);
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
