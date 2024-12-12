#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

void blink(unordered_map<unsigned long, size_t>& stones)
{
    auto digitCount = [](unsigned long stone)
    {
        return static_cast<unsigned long>(floor(log10(stone))+1);
    };

    auto oldStones = stones; // copy
    for(const auto& [stone, count] : oldStones)
    {
        if(count == 0)
        {
            continue;
        }
        if(stone == 0)
        {
            stones[1] += count;
            stones[0] -= count;
        }
        else if (auto n = digitCount(stone); n % 2 == 0)
        {
            // Using a slight optimization to avoid converting
            // back and forth between strings and numbers
            unsigned long divisor = pow(10, n / 2);
            stones[stone / divisor] += count;
            stones[stone % divisor] += count;
            stones[stone] -= count;
        }
        else
        {
            stones[stone * 2024] += count;
            stones[stone] -= count;
        }
    }
}

auto blink(const vector<unsigned long>& initialStones, unsigned int times)
{
    // Order actually isn't important for this problem...
    unordered_map<unsigned long, size_t> stones;
    for_each(initialStones.begin(), initialStones.end(), [&stones](auto& stone) { stones[stone] += 1; });
    for(auto i = 1u; i <= times; ++i)
    {
        blink(stones);
    }
    return accumulate(stones.begin(), stones.end(), 0ul, [](auto sum, const auto& pair) { return sum + pair.second; });
}

int main(int argc, char** argv)
{
    stringstream example1(R"(0 1 10 99 999)");
    stringstream example2(R"(125 17)");
    ifstream file("../day11.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    if(!getline(input, line))
    {
        cerr << "Failed to read file" << "\n";
        return 1;
    }

    vector<unsigned long> initialStones;
    for(auto&& range : line | views::split(' '))
    {
        string_view sv(&*range.begin(), ranges::distance(range));
        initialStones.emplace_back(stoul(string(sv)));
    }

    const auto part1 = blink(initialStones, 25);
    const auto part2 = blink(initialStones, 75);
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}
