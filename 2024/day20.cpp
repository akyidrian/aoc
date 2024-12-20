#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

enum Direction : size_t
{
    Left = 0,
    Down,
    Right,
    Up
};
constexpr int dx[]{1, 0, -1, 0};
constexpr int dy[]{0, 1, 0, -1};
constexpr Direction directions[]{Direction::Left, Direction::Down, Direction::Right, Direction::Up};

struct Vector2D
{
    int x;
    int y;

    Vector2D operator+(const Vector2D& other) const
    {
        return {x + other.x, y + other.y};
    }

    bool operator>(const Vector2D& other) const
    {
        return (x+y) > (other.x+other.y);
    }

    bool operator==(const Vector2D& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2D& other) const
    {
        return !(*this == other);
    }

    bool inBounds(const Vector2D& size) const
    {
        return x >= 0 && y >= 0 && x < size.x && y < size.y;
    }

    struct Hash {
        std::size_t operator()(const Vector2D& p) const {
            return hash<int>()(p.y) ^ (hash<int>()(p.x) << 1);
        }
    };
};

struct State
{
    Vector2D pos{}; // TODO: May be unnecessary
    unsigned int cost = 0;
    vector<Vector2D> path;

    bool operator>(const State& other) const
    {
        return cost > other.cost;
    }
};

pair<vector<Vector2D>, unsigned int> shortestPath(const vector<string>& racetrack, const Vector2D& startPosition, const Vector2D& endPosition)
{
    const Vector2D size{static_cast<int>(racetrack[0].size()), static_cast<int>(racetrack.size())};
    priority_queue<State, vector<State>, greater<State>> pq;
    unordered_set<Vector2D, Vector2D::Hash> visited;
    pq.emplace(startPosition, 0, vector<Vector2D>{startPosition});
    while(!pq.empty())
    {
        auto curr = pq.top();
        pq.pop();
        if(curr.pos == endPosition)
        {
            return {curr.path, curr.cost};
        }
        if(visited.find(curr.pos) != visited.end())
        {
            continue;
        }
        visited.emplace(curr.pos);
        for(const auto dir : directions)
        {
            const Vector2D direction = {dx[dir], dy[dir]};
            const auto next = curr.pos + direction;
            if(!next.inBounds(size) || racetrack[next.y][next.x] == '#')
            {
                continue;
            }
            auto path = curr.path;
            path.emplace_back(next);
            pq.emplace(next, curr.cost + 1, path);
        }
    }
    return {{}, 0u};
}

auto calculateCheats(const vector<string>& racetrack, const Vector2D& startPosition, const Vector2D& endPosition, unsigned int minTimeSavings)
{
    const Vector2D size{static_cast<int>(racetrack[0].size()), static_cast<int>(racetrack.size())};
    const auto [path, distance] = shortestPath(racetrack, startPosition, endPosition);
    unordered_map<Vector2D, unsigned int, Vector2D::Hash> pathMap;
    for(auto i = 0u; i < path.size(); ++i)
    {
        pathMap[path[i]] = i;
    }
    unordered_set<Vector2D, Vector2D::Hash> wallsBypassed;
    auto cheatsCounter = 0u;
    for(const auto& p : path)
    {
        for(const auto dir : directions)
        {
            const Vector2D direction = {dx[dir], dy[dir]};
            const auto next = p + direction;
            if(!next.inBounds(size) || wallsBypassed.find(next) != wallsBypassed.end())
            {
                continue;
            }
            const auto nextNext = p + direction + direction;
            if(racetrack[next.y][next.x] == '#' && pathMap.find(nextNext) != pathMap.end())
            {

                const auto nextNext = p + direction + direction;
                wallsBypassed.emplace(next);
                const auto timeSavings = pathMap[nextNext] - pathMap[p] - 2;
                cheatsCounter += timeSavings >= minTimeSavings;
            }
        }
    }
    return cheatsCounter;
}

int main(int argc, char** argv)
{
    stringstream example(
R"(###############
#...#...#.....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..E#...#...#
###.#######.###
#...###...#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############)");
    ifstream file("../day20.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> racetrack;
    while (getline(input, line))
    {
        racetrack.emplace_back(line);
    }

    int sx, sy, ex, ey;
    bool startFound = false;
    bool endFound = false;
    auto foundBoth = [&startFound, &endFound](){ return !startFound || !endFound; };
    for(auto y = 0; y < racetrack.size() && foundBoth(); ++y)
    {
        for(auto x = 0; x < racetrack.size() && foundBoth(); ++x)
        {
            if(racetrack[y][x] == 'S')
            {
                sx = x;
                sy = y;
                startFound = true;
                continue;
            }
            if(racetrack[y][x] == 'E')
            {
                ex = x;
                ey = y;
                endFound = true;
                continue;
            }
        }
    }
    cout << sx << "," << sy << endl;
    cout << ex << "," << ey << endl;
    for(const auto& l : racetrack)
    {
        cout << l << endl;
    }
    const auto cheatsCount = calculateCheats(racetrack, {sx, sy}, {ex, ey}, 100);
    cout << cheatsCount << endl;
}
