#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <sstream>
#include <unordered_set>

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

    bool inBounds(size_t size) const
    {
        return x >= 0 && y >= 0 && x < size && y < size;
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

auto part1(const unordered_set<Vector2D, Vector2D::Hash>& bytePositions, int memorySize)
{
    priority_queue<State, vector<State>, greater<State>> pq;
    unordered_set<Vector2D, Vector2D::Hash> visited;
    vector<string> map(memorySize, string(memorySize, '.'));
    for(const auto& p : bytePositions)
    {
        map[p.y][p.x] = '#';
    }
    const auto startPosition = Vector2D{0, 0};
    const auto endPosition = Vector2D{memorySize-1, memorySize-1};
    pq.emplace(startPosition, 0, vector<Vector2D>{startPosition});
    while(!pq.empty())
    {
        auto curr = pq.top();
        pq.pop();
        if(curr.pos == endPosition)
        {
            for(const auto& p : curr.path)
            {
                map[p.y][p.x] = 'O';
            }
            for(const auto& l : map)
            {
                cout << l << endl;
            }
            return curr.cost;
        }
        if(bytePositions.find(curr.pos) != bytePositions.end() || visited.find(curr.pos) != visited.end())
        {
            continue;
        }
        visited.emplace(curr.pos);
        for(const auto dir : directions)
        {
            const Vector2D direction = {dx[dir], dy[dir]};
            const auto next = curr.pos + direction;
            if(next.inBounds(memorySize))
            {
                auto path = curr.path;
                path.emplace_back(next);
                pq.emplace(next, curr.cost + 1, path);
            }
        }
    }
    for(const auto& l : map)
    {
        cout << l << endl;
    }
    return 0u;
}

auto findEndPosition(const unordered_set<Vector2D, Vector2D::Hash>& bytePositions, int memorySize)
{
    priority_queue<State, vector<State>, greater<State>> pq;
    unordered_set<Vector2D, Vector2D::Hash> visited;
    vector<string> map(memorySize, string(memorySize, '.'));
    for(const auto& p : bytePositions)
    {
        map[p.y][p.x] = '#';
    }
    const auto startPosition = Vector2D{0, 0};
    const auto endPosition = Vector2D{memorySize-1, memorySize-1};
    pq.emplace(startPosition, 0, vector<Vector2D>{startPosition});
    while(!pq.empty())
    {
        auto curr = pq.top();
        pq.pop();
        if(curr.pos == endPosition)
        {
            for(const auto& p : curr.path)
            {
                map[p.y][p.x] = 'O';
            }
//            for(const auto& l : map)
//            {
//                cout << l << endl;
//            }
            return curr.pos; // TODO:
        }
        if(bytePositions.find(curr.pos) != bytePositions.end() || visited.find(curr.pos) != visited.end())
        {
            continue;
        }
        visited.emplace(curr.pos);
        for(const auto dir : directions)
        {
            const Vector2D direction = {dx[dir], dy[dir]};
            const auto next = curr.pos + direction;
            if(next.inBounds(memorySize))
            {
                auto path = curr.path;
                path.emplace_back(next);
                pq.emplace(next, curr.cost + 1, path);
            }
        }
    }
//    for(const auto& l : map)
//    {
//        cout << l << endl;
//    }
    return Vector2D{0, 0};
}

auto part2(const vector<Vector2D>& bytePositions, int memorySize)
{
    auto endPosition = Vector2D(memorySize-1, memorySize-1);
    Vector2D coords = endPosition;
    auto i = memorySize-1;
    auto blocker = bytePositions.begin();
    while(coords == endPosition)
    {
        unordered_set<Vector2D, Vector2D::Hash> bytePositionsSet(bytePositions.begin(), bytePositions.begin() + i);
        coords = findEndPosition(bytePositionsSet, memorySize);
        blocker = bytePositions.begin() + i - 1;
        i++;
    }
    return *blocker;
}

int main(int argc, char** argv)
{
    const auto exampleBytes = 12u;
    const auto exampleMemorySize = 7u;
    stringstream example(
R"(5,4
4,2
4,5
3,0
2,1
6,3
2,4
1,5
0,6
3,3
2,6
5,1
1,2
5,5
2,5
6,5
1,4
0,4
6,4
1,1
6,1
1,0
0,5
1,6
2,0)");
    const auto fileBytes = 1024u;
    const auto fileMemorySize = 71u;
    ifstream file("../day18.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    const auto& bytes = fileBytes;
    const auto& memorySize = fileMemorySize;
    auto& input = file;
    string line;
    regex bytePositionRegex(R"((\d+),(\d+))");
    vector<Vector2D> bytePositions;
    while(getline(input, line))
    {
        smatch match;
        if(regex_match(line, match, bytePositionRegex))
        {
            bytePositions.emplace_back(stoi(match[1]), stoi(match[2]));
        }
    }

    unordered_set<Vector2D, Vector2D::Hash> bytePositionsSet(bytePositions.begin(), bytePositions.begin()+bytes);
    const auto steps = part1(bytePositionsSet, memorySize);
    const auto coords = part2(bytePositions, memorySize);
    cout << steps << endl;
    cout << coords.x << ", " <<  coords.y << endl;
}
