
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <ranges>
#include <vector>

using namespace std;

vector<string> generateCombinations(size_t n, const vector<char>& operators)
{
    std::vector<std::string> combinations;
    int k = operators.size();
    int total = std::pow(k, n);

    for (auto i = 0u; i < total; ++i)
    {
        std::string combination;
        auto value = i;
        for (auto j = 0u; j < n; ++j)
        {
            combination += operators[value % k];
            value /= k;
        }
        combinations.emplace_back(combination);
    }
    return combinations;
}

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
    auto& input = file;
    string line;
    vector<unsigned long> testValues;
    vector<vector<unsigned long>> operatorValuesList;
    while (getline(input, line)) {
        regex tokenRegex(R"((\d+):\s*((\d+\s*)+))");
        smatch match;
        if (std::regex_match(line, match, tokenRegex))
        {
            testValues.emplace_back(stoul(match[1]));
            string operatorValuesStr = match[2].str();
            vector<unsigned long> operatorValues;
            for (auto&& lineRange : operatorValuesStr | views::split(' '))
            {
                string_view sv(&*lineRange.begin(), ranges::distance(lineRange));
                operatorValues.emplace_back(stoul(string(sv)));
            }
            operatorValuesList.emplace_back(operatorValues);
        }
    }

    unsigned long totalCalibrationResult = 0u;
    const vector<char> operators{'+', '*'};
    for(auto i = 0u; i < testValues.size(); ++i)
    {
        const auto testValue = testValues[i];
        const auto operatorValues = operatorValuesList[i];
        const auto combinations = generateCombinations(operatorValues.size()-1, operators);
        for(const auto combination : combinations)
        {
            unsigned long candidateTestValue = operatorValues[0];
            for(auto j = 0u; j < combination.size(); ++j)
            {
                if(combination[j] == '+')
                {
                    candidateTestValue += operatorValues[j+1];
                }
                else if(combination[j] == '*')
                {
                    candidateTestValue *= operatorValues[j+1];
                }
            }
            if(testValue == candidateTestValue)
            {
                totalCalibrationResult += testValue;
                break;
            }
        }
    }
    cout << totalCalibrationResult << endl;
}
