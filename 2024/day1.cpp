#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    stringstream example(
    R"(3   4
4   3
2   5
1   3
3   9
3   3)");
    ifstream file("../day1.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<int> left;
    vector<int> right;
    while (getline(input, line)) {
        regex re("(\\d+)   (\\d+)");
        smatch match;
        if (regex_search(line, match, re)) {
            left.push_back(stoi(match[1]));
            right.push_back(stoi(match[2]));
        }
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    auto distance = 0u;
    for(auto i = 0u; i < min(left.size(), right.size()); ++i)
    {
        distance += abs(left[i] - right[i]);
    }
    cout << "Total distance: " << distance << "\n";

    auto similarity = 0u;
    for(auto l : left)
    {
        auto counter = 0u;
        for(auto i = 0u; i < right.size() && right[i] <= l; ++i)
        {
            counter += l==right[i];
        }
        similarity += l*counter;
    }
    cout << "Total similarity: " << similarity << "\n";
}
