#include <climits>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
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
    int x, y, dir, cost;
    bool operator>(const State& other) const
    {
        return cost > other.cost; // Required for min-heap priority queue
    }
};

// Using Uniform Cost Search, which is guaranteed to find the 'optimal' solution...
auto solveMaze(const vector<string> &maze, int sx, int sy, int ex, int ey)
{
    int rows = maze.size();
    int cols = maze[0].size();
    constexpr auto IntMax = numeric_limits<int>::max();

    priority_queue<State, vector<State>, greater<State>> pq;

    vector<vector<vector<int>>> visited(rows, vector<vector<int>>(cols, vector<int>(4, IntMax)));
    pq.emplace(sx, sy, Direction::East, 0);
    visited[sy][sx][Direction::East] = 0;
    while(!pq.empty())
    {
        State curr = pq.top();
        pq.pop();
        auto x = curr.x, y = curr.y, dir = curr.dir, cost = curr.cost;

        if(x == ex && y == ey)
        {
            return cost;
        }
        if(cost > visited[y][x][dir])
        {
            continue;
        }

        auto nx = x + dx[dir];
        auto ny = y + dy[dir];
        auto newCost = cost + 1;
        if(maze[ny][nx] != '#' && newCost < visited[ny][nx][dir])
        {
            visited[ny][nx][dir] = newCost;
            pq.emplace(nx, ny, dir, newCost);
        }

        auto leftDir = (dir + 3) % 4;
        auto newLeftCost = cost + 1000;
        if(newLeftCost < visited[y][x][leftDir])
        {
            visited[y][x][leftDir] = newLeftCost;
            pq.emplace(x, y, leftDir, newLeftCost);
        }
        auto rightDir = (dir + 1) % 4;
        auto newRightCost = cost + 1000;
        if(newRightCost < visited[y][x][rightDir])
        {
            visited[y][x][rightDir] = newRightCost;
            pq.emplace(x, y, rightDir, newRightCost);
        }
    }
    return IntMax;
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

    int sx, sy, ex, ey;
    bool startFound = false;
    bool endFound = false;
    auto foundBoth = [&startFound, &endFound](){ return !startFound || !endFound; };
    for(auto y = 0; y < maze.size() && foundBoth(); ++y)
    {
        for(auto x = 0; x < maze.size() && foundBoth(); ++x)
        {
            if(maze[y][x] == 'S')
            {
                sx = x;
                sy = y;
                startFound = true;
                continue;
            }
            if(maze[y][x] == 'E')
            {
                ex = x;
                ey = y;
                endFound = true;
                continue;
            }
        }
    }

    const auto lowestScore = solveMaze(maze, sx, sy, ex, ey);
    cout << lowestScore << endl;
}
