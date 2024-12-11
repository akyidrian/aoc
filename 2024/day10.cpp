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

enum class Direction : uint8_t
{
    Up = 0,
    Down,
    Left,
    Right
};
constexpr array<Position, 4> directionMap{{/*up*/{-1, 0}, /*down*/ {1, 0}, /*left*/ {0, -1}, /*right*/ {0, 1}}};
using direction_t = std::underlying_type_t<Direction>;

unsigned int getHeight(const vector<string>& topoMap, const Position& pos)
{
    return topoMap[pos.y][pos.x] - '0';
}

queue<Position> getTrailheads(const vector<string>& topoMap)
{
    const auto ySize = topoMap.size();
    const auto xSize = topoMap[0].size();
    queue<Position> trailheads;
    for(int y = 0; y < ySize; ++y)
    {
        for(int x = 0; x < xSize; ++x)
        {
            Position p{y,x};
            if(getHeight(topoMap, p) == 0) // trailheads are at height 0
            {
                trailheads.emplace(p);
            }
        }
    }
    return trailheads;
}

void stepTrailPath(queue<Position>& trailPositions,
                   unordered_set<Position, Position::Hash>& trailEndsSet,
                   vector<Position>& trailEndsVec,
                   const vector<string>& topoMap)
{
    const auto& current = trailPositions.front();
    trailPositions.pop();
    const auto ySize = topoMap.size();
    const auto xSize = topoMap[0].size();
    const auto currentHeight = getHeight(topoMap, current);
    for(const auto& dir : directionMap)
    {
        const auto next = current + dir;
        if(next.inBounds(ySize, xSize))
        {
            const auto nextHeight = getHeight(topoMap, next);
            const auto goingUp = (nextHeight - currentHeight) == 1;
            if(goingUp && nextHeight == 9) // reached the trail end?
            {
                trailEndsSet.emplace(next);
                trailEndsVec.emplace_back(next);
            }
            else if(goingUp)
            {
                trailPositions.emplace(next);
            }
        }
    }
}


auto calculateTotal(const vector<string>& topoMap, queue<Position> trailheads)
{
    pair<unsigned int, unsigned int> total; // score, rating
    while(!trailheads.empty())
    {
        // Possible positions between heads and tails
        queue<Position> trailPositions;
        trailPositions.push(trailheads.front());
        trailheads.pop();

        // For score - we use the unordered_set to find distinct trail ends.
        unordered_set<Position, Position::Hash> trailtailsSet;

        // For rating - we use vector to store duplicate trail ends, which may
        // represent more than one distinct path used to reach a trail end.
        vector<Position> trailtailsVec;

        stepTrailPath(trailPositions, trailtailsSet, trailtailsVec, topoMap);
        while(!trailPositions.empty())
        {
            stepTrailPath(trailPositions, trailtailsSet, trailtailsVec, topoMap);
        }
        total.first += trailtailsSet.size();
        total.second += trailtailsVec.size();
    }
    return total;
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

    auto trailheads = getTrailheads(topoMap);
    const auto [totalScore, totalRating] = calculateTotal(topoMap, trailheads);
    cout << totalScore << endl;
    cout << totalRating << endl;
}
