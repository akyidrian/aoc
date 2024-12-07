#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <ranges>
#include <vector>

using namespace std;

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

    struct SimulateGuard
    {
        struct Position
        {
            size_t x;
            size_t y;
        };
        enum class Direction
        {
            Up,
            Down,
            Left,
            Right
        };

        Position position{0, 0};
        Direction direction = Direction::Up;

        size_t xLabMapSize = 0;
        size_t yLabMapSize = 0;
        vector<string> labMap;
        bool printLabMapStates = false;

        SimulateGuard() = delete;
        SimulateGuard(const vector<string>& labMap, bool printLabMapStates = false) : xLabMapSize(labMap[0].size()), yLabMapSize(labMap.size()), labMap(labMap), printLabMapStates(printLabMapStates)
        {
            initializeGuardPosition();
        }

        void initializeGuardPosition()
        {
            for(auto y = 0u; y < yLabMapSize; ++y)
            {
                for(auto x = 0u; x < xLabMapSize; ++x)
                {
                    if(labMap[y][x] == '^')
                    {
                        position.x = x;
                        position.y = y;
                    }
                }
            }
        }

        Position nextPosition(const Position& current, const Direction& dir) const
        {
            Position next = current;
            if(dir == Direction::Up)
            {
                next.y -= 1;
            }
            else if(dir == Direction::Down)
            {
                next.y += 1;
            }
            else if(dir == Direction::Right)
            {
                next.x += 1;
            }
            else if(dir == Direction::Left)
            {
                next.x -= 1;
            }
            return next;
        }

        char currentGuardChar() const
        {
            char guardChar = '.';
            if(direction == Direction::Up)
            {
                guardChar = '^';
            }
            else if(direction == Direction::Down)
            {
                guardChar = 'v';
            }
            else if(direction == Direction::Right)
            {
                guardChar = '>';
            }
            else if(direction == Direction::Left)
            {
                guardChar = '<';
            }
            return guardChar;
        }

        void move()
        {
            const auto nextPos = nextPosition(position, direction);
            labMap[position.y][position.x] = 'X';
            labMap[nextPos.y][nextPos.x] = currentGuardChar();
            position = nextPos;
        }

        unsigned int countVisitedPositions() const
        {
            return accumulate(labMap.begin(), labMap.end(), 0u, [](auto acc, const std::string& line) {
                return acc + count(line.begin(), line.end(), 'X');
            });
        }

        unsigned int patrol()
        {
            printLabMap();
            while(!leavingArea())
            {
                if(obstructed())
                {
                    rotate(direction);
                }
                move();
                printLabMap();
            }
            labMap[position.y][position.x] = 'X';
            printLabMap();
            return countVisitedPositions(); 
        }

        void rotate(Direction& dir)
        {
            if(dir == Direction::Up)
            {
                dir = Direction::Right;
            }
            else if(dir == Direction::Right)
            {
                dir = Direction::Down;
            }
            else if(dir == Direction::Down)
            {
                dir = Direction::Left;
            }
            else if(dir == Direction::Left)
            {
                dir = Direction::Up;
            }
        }

        bool obstructed() const
        {
            const auto nextPos = nextPosition(position, direction);
            const auto c = labMap[nextPos.y][nextPos.x];
            return c == '#';
        }

        bool leavingArea() const
        {
            const auto leavingLeft = position.x == 0 && direction == Direction::Left;
            const auto leavingRight = position.x == xLabMapSize-1 && direction == Direction::Right;
            const auto leavingUp = position.y == 0 && direction == Direction::Up;
            const auto leavingDown = position.y == yLabMapSize-1 && direction == Direction::Down;
            return leavingLeft || leavingRight || leavingUp || leavingDown;
        }

        void printLabMap() const
        {
            if(!printLabMapStates)
            {
                return;
            }
            for(const auto& l : labMap)
            {
                cout << l << "\n";
            }
            cout << endl;
        }
    };

    SimulateGuard simulate(lines);
    cout << simulate.patrol() << endl;
}
