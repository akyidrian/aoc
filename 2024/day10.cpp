#include <array>
#include <fstream>
#include <iostream>
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

    bool inBounds(const int xSize, const int ySize) const
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

enum class Direction : uint8_t
{
    Up = 0,
    Down,
    Left,
    Right
};
constexpr array<Position, 4> directionMap{{/*up*/{-1, 0}, /*down*/ {1, 0}, /*left*/ {0, -1}, /*right*/ {0, 1}}};
using direction_t = std::underlying_type_t<Direction>;

unsigned int getHeight(const vector<string>& topographicMap, const Position& pos)
{
    return topographicMap[pos.y][pos.x] - '0';
}

int main(int argc, char** argv)
{
    stringstream example(
    R"(89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732
)");
    ifstream file("../day10.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> topoMap; // topographic map
    while (getline(input, line)) {
        topoMap.emplace_back(line);
    }

    const auto ySize = topoMap.size();
    const auto xSize = topoMap[0].size();
    queue<Position> trailheads;
    for(int y = 0; y < ySize; ++y)
    {
        for(int x = 0; x < xSize; ++x)
        {
            Position p{y,x};
            if(getHeight(topoMap, p) == 0)
            {
                trailheads.emplace(p);
            }
        }
    }

    auto stepTrailPath = [&topoMap](queue<Position>& trailPositions, unordered_set<Position, Position::Hash>& trailEnd, const Position& current)
    {
        const auto ySize = topoMap.size();
        const auto xSize = topoMap[0].size();
        const auto currentHeight = getHeight(topoMap, current);
        for(const auto& dir : directionMap)
        {
            const auto next = current + dir;
            if(next.inBounds(xSize, ySize))
            {
                const auto nextHeight = getHeight(topoMap, next);
                const auto goingUp = (nextHeight - currentHeight) == 1;
                if(goingUp && nextHeight == 9)
                {
                    // There may be more than one path to a trail end
                    // which is why we use an unordered_set
                    trailEnd.emplace(next);
                }
                else if(goingUp)
                {
                    trailPositions.emplace(next);
                }
            }
        }
    };

    auto totalScore = 0u;
    while(!trailheads.empty())
    {
        queue<Position> trailPositions;
        unordered_set<Position, Position::Hash> trailtail;
        stepTrailPath(trailPositions, trailtail, trailheads.front());
        while(!trailPositions.empty())
        {
            stepTrailPath(trailPositions, trailtail, trailPositions.front());
            trailPositions.pop();
        }
        totalScore += trailtail.size();
        trailheads.pop();
    }
    cout << totalScore << endl;
}
