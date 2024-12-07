
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <ranges>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    stringstream example(
    R"(190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20
)");
    ifstream file("../day7.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = example;
    string line;
    vector<int> testValues;
    vector<vector<int>> operatorValuesList;
    while (getline(input, line)) {
        regex tokenRegex(R"((\d+):\s*((\d+\s*)+))");
        smatch match;
        if (std::regex_match(line, match, tokenRegex))
        {
            testValues.emplace_back(stoi(match[1]));
            string operatorValuesStr = match[2].str();
            vector<int> operatorValues;
            for (auto&& lineRange : operatorValuesStr | views::split(' '))
            {
                string_view sv(&*lineRange.begin(), ranges::distance(lineRange));
                operatorValues.emplace_back(stoi(string(sv)));
            }
            operatorValuesList.emplace_back(operatorValues);
        }
    }
    for(auto t : testValues)
    {
        cout << t << endl;
    }
    for(auto o : operatorValuesList)
    {
        for(auto i : o)
        {
            cout << i << " ";
        }
        cout << endl;
    }
}
