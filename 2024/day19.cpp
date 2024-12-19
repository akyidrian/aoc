#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

bool wordBreak(const unordered_set<string>& patterns, const string& s, int start, unordered_map<int, bool>& memo)
{
    if (start == s.size())
    {
        return true; // If we've reached the end of the string
    }
    if (memo.find(start) != memo.end())
    {
        return memo[start]; // Return cached result if we've already solved this subproblem
    }

    // Try all possible splits
    for (int i = start + 1; i <= s.size(); ++i)
    {
        const string prefix = s.substr(start, i - start);
        if (patterns.find(prefix) != patterns.end() && wordBreak(patterns, s, i, memo))
        {
            memo[start] = true; // Memoize the result for this start position
            return true;
        }
    }

    memo[start] = false; // If no valid split found
    return false;
}

auto findDesignsPossible(const vector<string>& patterns, const vector<string>& designs)
{
    unordered_set<string> patternsSet(patterns.begin(), patterns.end());
    auto count = 0u;
    for(const auto& d : designs)
    {
        unordered_map<int, bool> memo; // Memoization map
        count += wordBreak(patternsSet, d, 0, memo);
    }
    return count;
}

int main(int argc, char** argv)
{
    stringstream example(
R"(r, wr, b, g, bwu, rb, gb, br

brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb)");
    ifstream file("../day19.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;

    // Parsing input
    getline(input, line);
    regex towelPatternsDelimiter(",\\s");
    sregex_token_iterator it(line.begin(), line.end(), towelPatternsDelimiter, -1);
    sregex_token_iterator end;
    vector<string> towelPatterns(it, end);
    getline(input, line); // Skip \n
    vector<string> towelDesigns;
    while(getline(input, line))
    {
        towelDesigns.emplace_back(line);
    }
//    for (const auto& p : towelPatterns)
//    {
//        cout << p << ", ";
//    }
//    cout << endl;
//
//    for (const auto& d : towelDesigns)
//    {
//        cout << d << ", ";
//    }
//    cout << endl;
    const auto designCount = findDesignsPossible(towelPatterns, towelDesigns);
    cout << designCount << endl;
}
