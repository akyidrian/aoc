#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <ranges>

using namespace std;

bool isOrderedUpdate(const multimap<int,int>& orderingRulesMap, const vector<int>& update)
{
    // Construct update map to represent it's ordering
    multimap<int,int> updateMap;
    for(auto i = 0u; i < update.size(); ++i)
    {
        for(auto j = i+1; j < update.size(); ++j)
        {
            updateMap.emplace(update[i], update[j]);
        }
    }

    // Count the number of matches between our update map and the ordering rules map
    // We need everything to match up from our update map to consider it correctly ordered
    auto orderedCounter = 0u;
    for(const auto& [x,y] : updateMap)
    {
        auto range = orderingRulesMap.equal_range(x);
        for (auto it = range.first; it != range.second; ++it) {
            if(y == it->second)
            {
                orderedCounter++;
                break;
            }
        }
    }
    return orderedCounter == updateMap.size();
}

vector<int> correctlyOrderUpdate(const multimap<int,int>& orderingRulesMap, const vector<int>& unorderedUpdate)
{
    auto orderedUpdate = unorderedUpdate;
    for(auto i = 0u; i < orderedUpdate.size(); ++i)
    {
        for(auto j = i+1; j < orderedUpdate.size(); ++j)
        {
            // Use the ordering rules of later page numbers (j) and check
            // if they're in the wrong position relative to current page number (i).
            // If they are in the wrong position (out of order), swap them.
            const auto x = orderedUpdate[j];
            const auto range = orderingRulesMap.equal_range(x);
            for (auto it = range.first; it != range.second; ++it) {
                const auto y = it->second;
                if(orderedUpdate[i] == y) // out of order?
                {
                    swap(orderedUpdate[i], orderedUpdate[j]);
                    break;
                }
            }
        }
    }
    return orderedUpdate;
}

void run(const vector<pair<int,int>>& orderingRules, const vector<vector<int>>& updates)
{
    multimap<int,int> orderingRulesMap;
    for(const auto& rule : orderingRules)
    {
        orderingRulesMap.emplace(rule);
    }

    auto sumPart1 = 0u;
    auto sumPart2 = 0u;
    for(const auto& update : updates)
    {
        if(isOrderedUpdate(orderingRulesMap, update))
        {
            sumPart1 += update[update.size() / 2];
        }
        else // not ordered
        {
            const auto orderedUpdate = correctlyOrderUpdate(orderingRulesMap, update);
            sumPart2 += orderedUpdate[orderedUpdate.size()/2];
        }
    }
    cout << "Sum of middle page numbers (part1): " << sumPart1 << "\n";
    cout << "Sum of middle page numbers (part2): " << sumPart2 << "\n";
}

int main(int argc, char** argv)
{
    stringstream example(
    R"(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47)");
    ifstream file("../day5.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> lines;
    while (getline(input, line)) {
        lines.emplace_back(line);
    }

    enum class Parsing
    {
        OrderingRules,
        Updates
    };
    auto parseMode = Parsing::OrderingRules;
    vector<pair<int,int>> orderingRules;
    vector<vector<int>> updates;
    for(const auto& l : lines)
    {
        if(l.empty())
        {
            // Empty line is the demarcation between ordering rules and updates
            parseMode = Parsing::Updates;
            continue;
        }
        if(parseMode == Parsing::OrderingRules)
        {
            // Assuming x, y are two digit numbers
            auto x = stoi(l.substr(0, 2));
            auto y = stoi(l.substr(3, 2));
            orderingRules.emplace_back(x, y);
        }
        else if(parseMode == Parsing::Updates) 
        {
            vector<int> update;
            for (auto&& lRange : l | views::split(',')) {
                string_view sv(&*lRange.begin(), ranges::distance(lRange));
                update.emplace_back(stoi(string(sv)));
            }
            updates.emplace_back(update);
        }
    }

    run(orderingRules, updates);
}
