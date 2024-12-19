#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Using backtracking with memoization 
unsigned long patternBreak(const unordered_set<string>& patterns, const string& s, size_t start, unordered_map<unsigned long, unsigned long>& memo)
{
    if (start == s.size())
    {
        return 1; // Reached the end of the string, one valid segmentation
    }

    if (memo.find(start) != memo.end())
    {
        return memo[start];
    }

    auto totalWays = 0ul;

    // Try all possible splits starting from `start`
    for (auto i = start + 1; i <= s.size(); ++i)
    {
        const string prefix = s.substr(start, i - start);
        if (patterns.find(prefix) != patterns.end())
        {
            // Recursively count all segmentations for the remainder of the string
            totalWays += patternBreak(patterns, s, i, memo);
        }
    }

    memo[start] = totalWays;
    return totalWays;
}

pair<unsigned long, unsigned long> findDesigns(const vector<string>& patterns, const vector<string>& designs)
{
    unordered_set<string> patternsSet(patterns.begin(), patterns.end());
    auto possibleCount = 0ul;
    auto differentWaysCount = 0ul;
    for(const auto& d : designs)
    {
        unordered_map<unsigned long, unsigned long> memo; // Memoization map
        const auto currDiffWays = patternBreak(patternsSet, d, 0, memo);
        possibleCount += currDiffWays > 0;
        differentWaysCount += currDiffWays; 
    }
    return {possibleCount, differentWaysCount};
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

    const auto [possibleCount, differentWaysCount] = findDesigns(towelPatterns, towelDesigns);
    cout << possibleCount << endl;
    cout << differentWaysCount << endl;
}
