#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
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
};

struct Guard
{
    enum class Direction : uint8_t
    {
        Up = 0,
        Down,
        Left,
        Right
    };
    using direction_t = std::underlying_type_t<Direction>;

    Position pos{.y = 0,.x = 0};
    Direction dir = Direction::Up;

    static constexpr array<char, 4> charMap{{'^', 'v', '<', '>'}};
    static constexpr array<Position, 4> dirMap{{/*up*/{-1, 0}, /*down*/ {1, 0}, /*left*/ {0, -1}, /*right*/ {0, 1}}};
    static constexpr array<direction_t, 4> nextDirLookup{{3, 2, 0, 1}};

    char character() const
    {
        return charMap[static_cast<direction_t>(dir)];
    }

    bool leavingArea(size_t yLabMapSize, size_t xLabMapSize)
    {
        return leavingArea(pos, dir, yLabMapSize, xLabMapSize);
    }

    bool leavingArea(Position& pos, const Direction& dir, size_t yLabMapSize, size_t xLabMapSize) const
    {
        const auto leavingUp = pos.y == 0 && dir == Direction::Up;
        const auto leavingDown = pos.y == yLabMapSize-1 && dir == Direction::Down;
        const auto leavingLeft = pos.x == 0 && dir == Direction::Left;
        const auto leavingRight = pos.x == xLabMapSize-1 && dir == Direction::Right;
        return leavingLeft || leavingRight || leavingUp || leavingDown;
    }

    void step()
    {
        pos = pos + dirMap[static_cast<direction_t>(dir)];
    }

    Position nextPos()
    {
        return pos + dirMap[static_cast<direction_t>(dir)];
    }

    void rotate()
    {
        dir = static_cast<Direction>(nextDirLookup[static_cast<direction_t>(dir)]);
    }

    Direction nextDir()
    {
        return static_cast<Direction>(nextDirLookup[static_cast<direction_t>(dir)]);
    }

    // Overload operator<< to print Guard state
    friend ostream& operator<<(ostream& os, const Guard& g)
    {
        os << "Position: (" << g.pos.y << ", " << g.pos.x << "), " << "Direction: " << g.character();
        return os;
    }
};

class SimulateLab
{
  public:
    SimulateLab() = delete;
    SimulateLab(vector<string> labMap)
    : m_labMap(labMap), m_xlabMapSize(labMap[0].size()), m_ylabMapSize(labMap.size())
    {
        initializeGuard();
    }

    unsigned int patrol()
    {
        auto count = 0u;
        while(!m_guard.leavingArea(m_ylabMapSize, m_xlabMapSize))
        {
            while(labMapChar(m_guard.nextPos()) == '#')
            {
                m_guard.rotate();
            }
            updateLabMap(m_guard.pos, 'X');
            m_guard.step();
            updateLabMap(m_guard.pos, m_guard.character());
        }
        updateLabMap(m_guard.pos, 'X'); // mark the final position before leaving lab
        return distinctPositionsVisited();
    }

    void printLabMap() const
    {
        for(const auto& l : m_labMap)
        {
            cout << l << "\n";
        }
        cout << endl;
    }

private:
    void initializeGuard()
    {
        for(auto y = 0; y < m_ylabMapSize; ++y)
        {
            for(auto x = 0; x < m_xlabMapSize; ++x)
            {
                if(m_labMap[y][x] == '^')
                {
                    m_guard.pos = {.y = y, .x = x};
                    m_guard.dir = Guard::Direction::Up;
                }
            }
        }
    }

    char labMapChar(const Position& p)
    {
        return m_labMap[p.y][p.x];
    }

    void updateLabMap(const Position& p, const char c)
    {
        m_labMap[p.y][p.x] = c;
    }

    unsigned int distinctPositionsVisited() const
    {
        auto sumOverRow = [](auto sum, const auto& l)
        {
            return sum + std::count(l.begin(), l.end(), 'X');
        };
        return accumulate(m_labMap.begin(), m_labMap.end(), 0u, sumOverRow);
    }

    Guard m_guard;
    vector<string> m_labMap;
    const size_t m_xlabMapSize;
    const size_t m_ylabMapSize;
};

int main(int argc, char** argv)
{
    stringstream example(
    R"(....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...
)");
    ifstream file("../day6.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> lines;
    while (getline(input, line)) {
        lines.emplace_back(line);
    }

    SimulateLab part1(lines);
    part1.printLabMap();
    cout << part1.patrol() << endl;
    //SimulateLab part2(lines);
    //cout << part2.obstructedPatrols() << endl;
}
