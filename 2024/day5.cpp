#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <ranges>

using namespace std;

unsigned int part1(const vector<pair<int,int>>& orders, const vector<vector<int>>& updates)
{
    auto sum = 0u;
    multimap<int,int> orderMap;
    for(auto& [x,y] : orders)
    {
        orderMap.insert({x,y});
    }
    for(auto& u : updates)
    {
        multimap<int,int> updateMap;
        for(auto i = 0u; i < u.size(); ++i)
        {
            for(auto j = i+1; j < u.size(); ++j)
            {
                updateMap.insert({u[i], u[j]});
            }
        }
        auto updateMapCounter = 0u;
        for(const auto& [k,v] : updateMap)
        {
            auto range = orderMap.equal_range(k);
            for (auto it = range.first; it != range.second; ++it) {
                if(v == it->second)
                {
                    updateMapCounter++;
                    break;
                }
            }
        }
        if(updateMapCounter == updateMap.size())
        {
            std::cout << u[u.size() / 2] << endl;
            sum += u[u.size() / 2];
        }
    }
    return sum;
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
        lines.push_back(line);
    }

    bool parseOrders = true; // false means parse updates
    vector<pair<int,int>> orders;
    vector<vector<int>> updates;
    for(auto& l : lines)
    {
        if(l.empty())
        {
            parseOrders = false;
            continue;
        }
        if(parseOrders)
        {
            auto x = stoi(l.substr(0, 2));
            auto y = stoi(l.substr(3, 2));
            orders.push_back({x, y});
        }
        else
        {
            vector<int> update;
            for (auto&& lRange : l | views::split(',')) {
                string_view sv(&*lRange.begin(), ranges::distance(lRange));
                update.push_back(stoi(string(sv)));
            }
            updates.push_back(update);
        }
    }

    std::cout << updates.size() << endl;

    auto sumPart1 = part1(orders, updates);
    cout << "Sum of middle page numbers (part1): " << sumPart1 << "\n";
}
