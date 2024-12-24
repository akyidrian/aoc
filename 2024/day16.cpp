#include <climits>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

enum Direction : size_t
{
    East = 0,
    South,
    West,
    North
};
constexpr int dx[]{1, 0, -1, 0};
constexpr int dy[]{0, 1, 0, -1};

struct State
{
    vector<pair<int,int>> path;
    int dir, cost;
    bool operator>(const State& other) const
    {
        return cost > other.cost; // Required for min-heap priority queue
    }
};

struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator ()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

pair<int,int> findCharPos(const vector<string>& maze, char c)
{
    for(auto y = 0; y < maze.size(); ++y)
    {
        for(auto x = 0; x < maze[0].size(); ++x)
        {
            if(maze[y][x] == c)
            {
                return {x,y};
            }
        }
    }
    return {-1,-1}; // Not found
}

// Using Uniform Cost Search, which is guaranteed to find the 'optimal' solutions...
auto solveMaze(vector<string> &maze, int sx, int sy, int ex, int ey)
{
    constexpr auto IntMax = numeric_limits<int>::max();
    constexpr auto StepCost  = 1;
    constexpr auto RotateCost = 1000;
    int ySize = maze.size();
    int xSize = maze[0].size();

    priority_queue<State, vector<State>, greater<State>> pq;
    pq.emplace(vector<pair<int,int>>{{sx,sy}}, Direction::East, 0);

    // TODO: More memory efficient to use an unordered_map
    vector<vector<vector<int>>> visited(ySize, vector<vector<int>>(xSize, vector<int>(4, IntMax)));
    visited[sy][sx][Direction::East] = 0;

    auto lowestScore = IntMax; // aka lowest cost
    unordered_set<pair<int,int>, pair_hash> bestTiles; // tiles in the best paths through the maze
    while(!pq.empty())
    {
        State curr = pq.top();
        pq.pop();
        auto [x,y] = curr.path.back();
        auto dir = curr.dir, cost = curr.cost;

        if(x == ex && y == ey)
        {
            if(curr.cost < lowestScore)
            {
                bestTiles.clear();
                for (const auto& p : curr.path) { bestTiles.emplace(p); }
                lowestScore = curr.cost;
            }
            else if(curr.cost == lowestScore)
            {
                for (const auto& p : curr.path) { bestTiles.emplace(p); }
            }
            continue;
        }
        if(cost > visited[y][x][dir])
        {
            continue;
        }

        const auto nx = x + dx[dir];
        const auto ny = y + dy[dir];
        const auto newCost = cost + StepCost;
        if(maze[ny][nx] != '#' && newCost <= visited[ny][nx][dir])
        {
            visited[ny][nx][dir] = newCost;
            auto path = curr.path;
            path.emplace_back(nx,ny);
            pq.emplace(path, dir, newCost);
        }

        const auto leftDir = (dir + 3) % 4;
        const auto newLeftCost = cost + RotateCost;
        if(newLeftCost <= visited[y][x][leftDir])
        {
            visited[y][x][leftDir] = newLeftCost;
            pq.emplace(curr.path, leftDir, newLeftCost);
        }
        const auto rightDir = (dir + 1) % 4;
        const auto newRightCost = cost + RotateCost;
        if(newRightCost <= visited[y][x][rightDir])
        {
            visited[y][x][rightDir] = newRightCost;
            pq.emplace(curr.path, rightDir, newRightCost);
        }
    }
    return pair<int,int>{lowestScore, bestTiles.size()};
}

int main(int argc, char** argv)
{
    stringstream example1(
R"(###############
#.......#....E#
#.#.###.#.###.#
#.....#.#...#.#
#.###.#####.#.#
#.#.#.......#.#
#.#.#####.###.#
#...........#.#
###.#.#####.#.#
#...#.....#.#.#
#.#.#.###.#.#.#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############)");
    stringstream example2(
R"(#################
#...#...#...#..E#
#.#.#.#.#.#.#.#.#
#.#.#.#...#...#.#
#.#.#.#.###.#.#.#
#...#.#.#.....#.#
#.#.#.#.#.#####.#
#.#...#.#.#.....#
#.#.#####.#.###.#
#.#.#.......#...#
#.#.###.#####.###
#.#.#...#.....#.#
#.#.#.#####.###.#
#.#.#.........#.#
#.#.#.#########.#
#S#.............#
#################)");
    ifstream file("../day16.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> maze;
    while (getline(input, line))
    {
        maze.emplace_back(line);
    }

    const auto [sx,sy] = findCharPos(maze, 'S');
    const auto [ex,ey] = findCharPos(maze, 'E');
    const auto [lowestScore,bestTiles] = solveMaze(maze, sx, sy, ex, ey);
    cout << lowestScore << endl;
    cout << bestTiles << endl;
}
