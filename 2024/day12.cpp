#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Used to help find all of a regions sides
class WeightedQuickUnionUF
{
private:
    vector<int> id; // Parent link (site indexed)
    vector<int> sz; // Size of component for roots (site indexed)
    int count; // Number of components

     // Follow links to find a root.
    int find(int p)
    {
        while (p != id[p])
        {
            id[p] = id[id[p]]; // Path compression
            p = id[p];
        }
        return p;
    }
public:
    WeightedQuickUnionUF(int N) : id(N), sz(N), count(N)
    {
        for (int i = 0; i < N; i++) id[i] = i;
        for (int i = 0; i < N; i++) sz[i] = 1;
    }

    int numberOfComponents()
    { return count; }

    bool connected(int p, int q)
    { return find(p) == find(q); }

    void unite(int p, int q)
    {
        int i = find(p);
        int j = find(q);
        if (i == j) return;
        // Make smaller root point to larger one.
        if (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
        else { id[j] = i; sz[i] += sz[j]; }
        count--;
    }
};

struct Region 
{
    char type;
    size_t area; // Number of garden plots the region contains
    size_t perimeter; // Number of sides of garden plots in the region that do not touch another garden plot in the same region
    size_t sides; // Number of sides of the region (straight sections of fencing)
};

struct Position
{
    int y = 0;
    int x = 0;

    bool neighbors(const Position& other) const;

    Position operator+(const Position& other) const
    {
        return Position{.y = y + other.y, .x = x + other.x};
    }

    void operator+=(const Position& other)
    {
        y += other.y;
        x += other.x;
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
constexpr array<Direction, 4> allDirections{Direction::Up, Direction::Down, Direction:: Left, Direction::Right};
using direction_t = std::underlying_type_t<Direction>;

constexpr array<Position, 4> directionMap{{/*up*/{-1, 0}, /*down*/ {1, 0}, /*left*/ {0, -1}, /*right*/ {0, 1}}};
using PerimeterMap = unordered_map<Direction, vector<Position>>; // To help figure out the sides of the region

bool Position::neighbors(const Position& other) const
{
    for(const auto dir : directionMap)
    {
        if(*this == (other + dir))
        {
            return true;
        }
    }
    return false;
}

char getPlot(const vector<string>& farm, const Position& pos)
{
    return farm[pos.y][pos.x];
}

void stepRegion(queue<Position>& nextRegionPlots, PositionSet& regionPlots, unordered_map<Direction, vector<Position>>& regionPerimeterMap, const vector<string>& farm)
{
    const auto yFarmSize = farm.size();
    const auto xFarmSize = farm[0].size();

    const auto& current = nextRegionPlots.front();
    nextRegionPlots.pop();

    const auto currentPlot = getPlot(farm, current);
    for(const auto& direction : allDirections)
    {
        const auto dir = directionMap[static_cast<size_t>(direction)];
        const auto next = current + dir;
        if(!next.inBounds(yFarmSize, xFarmSize))
        {
            regionPerimeterMap[direction].emplace_back(current);
            continue;
        }
        const auto visited = regionPlots.find(next) != regionPlots.end();
        if(visited) // Already visited this plot in this region?
        {
            continue; // Don't want to revisit...
        }

        const auto sameRegion = currentPlot == getPlot(farm, next);
        if(!sameRegion)
        {
            regionPerimeterMap[direction].emplace_back(current);
            continue;
        }

        // This plot is in the farm, hasn't been visited and in our current region...
        regionPlots.emplace(next);
        nextRegionPlots.emplace(next);
    }
}

auto countPerimeter(const PerimeterMap& regionPerimeterMap)
{
    return accumulate(
        regionPerimeterMap.begin(),
        regionPerimeterMap.end(),
        0u,
        [](auto sum, const auto& pair) { return sum + pair.second.size(); }
    );
}

auto countSides(const PerimeterMap& regionPerimeterMap)
{
    auto sides = 0u;
    for(const auto& [_, perimeter] : regionPerimeterMap)
    {
        auto uf = WeightedQuickUnionUF(perimeter.size());
        for(auto i = 0; i < perimeter.size(); ++i)
        {
            for(auto j = i+1; j < perimeter.size(); ++j)
            {
                // Do i and j neighbor each other?
                if(perimeter[i].neighbors(perimeter[j]))
                {
                    uf.unite(i, j);
                }
            }
        }
        sides += uf.numberOfComponents();
    }
    return sides;
}

Region findRegion(PositionSet& farmPlotVisited, const vector<string>& farm, const Position& start)
{
    queue<Position> nextRegionPlots; // To step through and process
    nextRegionPlots.emplace(start);
    PositionSet regionPlots; // Helps track what we've already pushed in the queue
    regionPlots.emplace(start);

    // Directions represent the direction we'd take from an edge plot position to exit the region.
    // Directions keys helps group edge plots positions that potentially establish a side to the region.
    PerimeterMap regionPerimeterMap
    {
        {Direction::Up, {}},
        {Direction::Down, {}},
        {Direction::Left, {}},
        {Direction::Right, {}}
    };
    while(!nextRegionPlots.empty())
    {
        stepRegion(nextRegionPlots, regionPlots, regionPerimeterMap, farm);
    }
    farmPlotVisited.insert(regionPlots.begin(), regionPlots.end());
    const auto area = regionPlots.size();
    const auto perimeter = countPerimeter(regionPerimeterMap);
    const auto sides = countSides(regionPerimeterMap);
    return {.type = getPlot(farm, start), .area = area, .perimeter = perimeter, .sides = sides};
}

vector<Region> findRegions(const vector<string>& farm)
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
            if(!visited) // Already visited this plot (/region)
            {
                regions.emplace_back(findRegion(farmPlotVisited, farm, p));
            }
        }
    }
    return regions;
}

auto calculateTotalPricePart1(const vector<Region>& regions)
{
    return accumulate(regions.begin(), regions.end(), 0ul, [](auto sum, const Region& r){ return sum + (r.area * r.perimeter); });
}

auto calculateTotalPricePart2(const vector<Region>& regions)
{
    return accumulate(regions.begin(), regions.end(), 0ul, [](auto sum, const Region& r){ return sum + (r.area * r.sides); });
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

    const auto regions = findRegions(farm);
    cout << calculateTotalPricePart1(regions) << endl;
    cout << calculateTotalPricePart2(regions) << endl;
}
