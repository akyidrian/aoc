#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <tuple>
#include <unordered_set>
#include <vector>

using namespace std;

struct Position
{
    int y = 0;
    int x = 0;

    Position operator+(const Position& other) const
    {
        return Position{.y = y + other.y, .x = x + other.x};
    }

    Position operator+=(const Position& other) const
    {
        return Position{.y = y + other.y, .x = x + other.x};
    }

    bool operator<(const Position& other) const
    {
        return std::tie(y, x) < std::tie(other.y, other.x);
    }

    bool inBounds(const int ySize, const int xSize) const
    {
        return x >= 0 && y >= 0 && x < xSize && y < ySize;
    }

    bool operator==(const Position& other) const {
        return y == other.y && x == other.x;
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& pos)
    {
        os << "(" << pos.y << ", " << pos.x << ")";
        return os;
    }

    struct Hash {
        std::size_t operator()(const Position& p) const {
            // Combine the two integers into a single hash
            return hash<int>()(p.y) ^ (hash<int>()(p.x) << 1);
        }
    };
};
using PositionSet = unordered_set<Position, Position::Hash>;

enum class Direction : uint8_t
{
    Up = 0,
    Down,
    Left,
    Right
};
constexpr array<Position, 4> directionMap{{/*up*/{-1, 0}, /*down*/ {1, 0}, /*left*/ {0, -1}, /*right*/ {0, 1}}};
using direction_t = std::underlying_type_t<Direction>;

char getPlot(const vector<string>& farm, const Position& pos)
{
    return farm[pos.y][pos.x];
}

struct Region 
{
    char type;
    unsigned int area;
    unsigned int perimeter;
};

auto stepRegion(queue<Position>& regionPositions, PositionSet& plotVisited, const vector<string>& farm)
{
    const auto ySize = farm.size();
    const auto xSize = farm[0].size();

    const auto& current = regionPositions.front();
    regionPositions.pop();

    const auto currentPlot = getPlot(farm, current);
    auto perimeter = 0u;
    for(const auto& dir : directionMap)
    {
        const auto next = current + dir;
        if(!next.inBounds(ySize, xSize))
        {
            perimeter += 1;
            continue;
        }
        const auto visited = plotVisited.find(next) != plotVisited.end();
        if(visited)
        {
            continue;
        }

        const auto sameRegion = currentPlot == getPlot(farm, next);
        if(!sameRegion)
        {
            perimeter += 1;
            continue;
        }
        regionPositions.emplace(next);
        plotVisited.emplace(next);
    }
    return pair<unsigned int, unsigned int>{1, perimeter};
}

Region findRegion(PositionSet& farmPlotVisited, const vector<string>& farm, const Position& start)
{
    queue<Position> regionPositions;
    regionPositions.emplace(start);
    PositionSet regionPlotVisited;
    regionPlotVisited.emplace(start);

    auto area = 0u;
    auto perimeter = 0u;
    while(!regionPositions.empty())
    {
        auto [stepArea, stepPerimeter] = stepRegion(regionPositions, regionPlotVisited, farm);
        area += stepArea;
        perimeter += stepPerimeter;
    }
    farmPlotVisited.insert(regionPlotVisited.begin(), regionPlotVisited.end());
    return {.type = getPlot(farm, start), .area = area, .perimeter = perimeter};
}

unsigned long calculateTotalPrice(const vector<string>& farm)
{
    const auto yFarmSize = farm.size();
    const auto xFarmSize = farm[0].size();
    PositionSet farmPlotVisited;
    vector<Region> regions;
    for(auto y = 0; y < yFarmSize; ++y)
    {
        for(auto x = 0; x < xFarmSize; ++x)
        {
            Position p{y,x};
            const auto visited = farmPlotVisited.find(p) != farmPlotVisited.end();
            if(!visited)
            {
                regions.emplace_back(findRegion(farmPlotVisited, farm, p));
            }
        }
    }
    return accumulate(regions.begin(), regions.end(), 0ul, [](auto sum, const Region& r){ return sum + (r.area * r.perimeter); });
}

int main(int argc, char** argv)
{
    // Part 1 Examples
    stringstream example1(R"(AAAA
BBCD
BBCC
EEEC)");
    stringstream example2(R"(OOOOO
OXOXO
OOOOO
OXOXO
OOOOO)");
    stringstream example3(R"(RRRRIICCFF
RRRRIICCCF
VVRRRCCFFF
VVRCCCJFFF
VVVVCJJCFE
VVIVCCJJEE
VVIIICJJEE
MIIIIIJJEE
MIIISIJEEE
MMMISSJEEE)");

    // Part 2 Examples
    stringstream example4(R"(EEEEE
EXXXX
EEEEE
EXXXX
EEEEE)");
    stringstream example5(R"(AAAAAA
AAABBA
AAABBA
ABBAAA
ABBAAA
AAAAAA)");
    ifstream file("../day12.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> farm;
    while (getline(input, line))
    {
        farm.emplace_back(line);
    }

    const auto totalPrice = calculateTotalPrice(farm);
    cout << totalPrice << endl;
}
