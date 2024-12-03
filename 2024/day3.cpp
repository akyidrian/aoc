#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <vector>

using namespace std;

int part1(const string& line)
{
    vector<pair<int,int>> nums;
    regex tokenRegex(R"(mul\((\d{1,3}),(\d{1,3})\))");
    sregex_iterator begin(line.begin(), line.end(), tokenRegex);
    sregex_iterator end;

    for(auto it = begin; it != end; ++it)
    {
        smatch match = *it;
        nums.push_back({stoi(match[1]), stoi(match[2])});
    }

    vector<int> products(nums.size());
    transform(nums.begin(), nums.end(), products.begin(), [](const pair<int, int>& p){ return p.first * p.second; });
    return accumulate(products.begin(), products.end(), 0);
}

int main(int argc, char** argv)
{
    stringstream example(
    R"(xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5)))");
    ifstream file("../day3.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = example;
    string line; // aka corrupted memory
    auto totalSumPart1 = 0;
    while (getline(input, line)) {
        totalSumPart1 += part1(line);
    }
    cout << totalSumPart1 << "\n";
}
