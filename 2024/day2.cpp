#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <ranges>
#include <vector>

using namespace std;

bool part1(vector<int>& levels)
{
    auto increasing = [](int a, int b){
        return (a - b) <= -1 && (a - b) >= -3;
    };
    auto decreasing = [](int a, int b){
        return (a - b) >= 1 && (a - b) <= 3;
    };
    auto incCounter = 0u;
    auto decCounter = 0u;
    for(size_t i = 1; i < levels.size(); ++i)
    {
        auto a = levels[i-1];
        auto b = levels[i];
        incCounter += increasing(a, b);
        decCounter += decreasing(a, b);
    }
    return incCounter==(levels.size()-1) || decCounter==(levels.size()-1);
}

bool allIncreasing(const vector<int>& levels)
{
    auto increasing = [](int a, int b){
        return (a - b) <= -1 && (a - b) >= -3;
    };
    for(auto i = 1u; i < levels.size(); ++i)
    {
        if(!increasing(levels[i-1], levels[i]))
        {
            return false;
        }
    }
    return true;
}

bool allDecreasing(const vector<int>& levels)
{
    auto decreasing = [](int a, int b){
        return (a - b) >= 1 && (a - b) <= 3;
    };
    for(auto i = 1u; i < levels.size(); ++i)
    {
        if(!decreasing(levels[i-1], levels[i]))
        {
            return false;
        }
    }
    return true;
}

bool part2(const vector<int>& levels)
{
    if(allIncreasing(levels) || allDecreasing(levels))
    {
        return true;
    }
    auto skip = 0u;
    while(skip < levels.size())
    {
        vector<int> newLevels;
        for(auto i = 0u; i < levels.size(); ++i)
        {
            if(i == skip)
            {
                continue;
            }
            newLevels.push_back(levels[i]);
        }
        if(allIncreasing(newLevels) || allDecreasing(newLevels))
        {
            return true;
        }
        skip++;
    }
    return false;
}

int main(int argc, char** argv)
{
    stringstream example(
    R"(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9)");
    ifstream file("../day2.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line; // aka reports
    auto safePart1 = 0u;
    auto safePart2 = 0u;
    while (getline(input, line)) {
        vector<int> levels;
        for (auto&& lineRange : line | views::split(' ')) {
            string_view sv(&*lineRange.begin(), ranges::distance(lineRange));
            levels.push_back(stoi(string(sv)));
        }
        safePart1 += part1(levels);
        safePart2 += part2(levels);
    }
    cout << "Part 1 safe reports: " << safePart1 << "\n";
    cout << "Part 2 safe reports:" << safePart2 << "\n";
}
