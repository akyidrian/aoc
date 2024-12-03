#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <vector>

using namespace std;

int sumOfProducts(int acc, const std::pair<int, int>& p) {
    return acc + (p.first * p.second);
}

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
    return accumulate(nums.begin(), nums.end(), 0, sumOfProducts);
}

int part2(const string& line)
{
    static auto doCommand = true;  // do / don't commands persist across corrupted memory lines

    vector<pair<int,int>> nums;
    regex tokenRegex(R"(do\(\)|don't\(\)|mul\((\d{1,3}),(\d{1,3})\))");
    sregex_iterator begin(line.begin(), line.end(), tokenRegex);
    sregex_iterator end;
    for(auto it = begin; it != end; ++it)
    {
        smatch match = *it;
        const string command(match[0]);
        if(command == "do()")
        {
            doCommand = true;
        }
        else if(command == "don't()")
        {
            doCommand = false;
        }
        else if(doCommand)
        {
            nums.push_back({stoi(match[1]), stoi(match[2])});
        }
    }
    return accumulate(nums.begin(), nums.end(), 0, sumOfProducts);
}

int main(int argc, char** argv)
{
    stringstream exampleP1(R"(xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5)))");
    stringstream exampleP2(R"(xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5)))");
    ifstream file("../day3.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line; // aka corrupted memory
    auto totalSumPart1 = 0;
    auto totalSumPart2 = 0;
    while (getline(input, line)) {
        totalSumPart1 += part1(line);
        totalSumPart2 += part2(line);
    }
    cout << totalSumPart1 << "\n";
    cout << totalSumPart2 << "\n";
}
