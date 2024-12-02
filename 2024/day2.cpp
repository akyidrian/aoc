#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <span>
#include <sstream>
#include <ranges>
#include <vector>

using namespace std;

bool increasing(int a, int b)
{
    return (a - b) <= -1 && (a - b) >= -3;
}

bool decreasing(int a, int b)
{
    return (a - b) >= 1 && (a - b) <= 3;
}

bool allIncreasing(span<const int>& levels)
{
    return ranges::adjacent_find(levels, not_fn(increasing)) == levels.end();
}

bool allDecreasing(span<const int>& levels)
{
    return ranges::adjacent_find(levels, not_fn(decreasing)) == levels.end();
}

bool part1(vector<int>& levels)
{
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


bool part2(const vector<int>& levels)
{
    span s(levels);
    if(allIncreasing(s) || allDecreasing(s))
    {
        return true;
    }
    auto skip = 0u;
    while(skip < levels.size())
    {
        // Dividing report into two halves around the skip point...
        span left(levels.begin(), skip);
        span right(levels.begin()+skip+1, levels.end());
        auto leftRightIncreasing = !left.size() || !right.size() || increasing(left.back(), right.front());
        auto leftRightDecreasing = !left.size() || !right.size() || decreasing(left.back(), right.front());
        if(leftRightIncreasing && allIncreasing(left) && allIncreasing(right))
        {
            return true;
        }
        else if(leftRightDecreasing && allDecreasing(left) && allDecreasing(right))
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
    cout << "Part 2 safe reports: " << safePart2 << "\n";
}
