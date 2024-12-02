#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <ranges>
#include <vector>

using namespace std;

bool part1(vector<int>& levels)
{
    auto increasing = (levels[0]-levels[1]) < 0;
    auto decreasing = (levels[0]-levels[1]) > 0;
    auto counter = 0u;
    for(auto i = 1; i < levels.size(); ++i)
    {
        auto diff = levels[i-1]-levels[i];
        if(increasing && diff <= -1 && diff >= -3)
        {
            counter++;
        }
        else if(decreasing && diff <= 3 && diff >= 1)
        {
            counter++;
        }
        else
        {
            break;
        }
    }
    return counter==(levels.size()-1);
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
    while (getline(input, line)) {
        vector<int> levels;
        for (auto&& lineRange : line | views::split(' ')) {
            string_view sv(&*lineRange.begin(), ranges::distance(lineRange));
            levels.push_back(stoi(string(sv)));
        }
        safePart1 += part1(levels);
    }
    cout << "Safe reports: " << safePart1 << "\n";
}
