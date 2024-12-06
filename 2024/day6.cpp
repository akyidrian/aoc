#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
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
    struct Guard
    {
        Position position{0, 0};
        size_t xLabSize = 0;
        size_t yLabSize = 0;
        vector<string> labMap;
        Direction direction = Direction::Up; // starting direction

        Guard() = delete;
        Guard(const vector<string> labMap) : xLabSize(labMap[0].size()), yLabSize(labMap.size()), labMap(labMap)
        {
            startingGuardPosition();
        }

        void startingGuardPosition()
        {
            for(auto y = 0u; y < labMap.size(); ++y)
            {
                for(auto x = 0u; x < labMap[0].size(); ++x)
                {
                    if(labMap[y][x] == '^')
                    {
                        position.x = x;
                        position.y = y;
                    }
                }
            }
        }

        Position nextPosition()
        {
            Position nextPosition = position;
            if(direction == Direction::Up)
            {
                nextPosition.y -= 1;
            }
            else if(direction == Direction::Down)
            {
                nextPosition.y += 1;
            }
            else if(direction == Direction::Right)
            {
                nextPosition.x += 1;
            }
            else if(direction == Direction::Left)
            {
                nextPosition.x -= 1;
            }
            return nextPosition;
        }

        char currentGuardChar()
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
            const auto nextPos = nextPosition();
            labMap[position.y][position.x] = 'X';
            labMap[nextPos.y][nextPos.x] = currentGuardChar();
            position = nextPos;
        }

        unsigned int countX()
        {
            auto count = 0;
            for(const auto& l : labMap)
            {
                for(const auto& c : l)
                {
                    if(c == 'X')
                    {
                        count++;
                    }
                }
            }
            return count;
        }

        unsigned int distinctPositions()
        {
            //printLabMap();
            while(!leavingArea())
            {
                if(obstructed())
                {
                    rotate();
                }
                move();
                //printLabMap();
            }
            labMap[position.y][position.x] = 'X';
            //printLabMap();
            return countX(); 
        }

        void rotate()
        {
            if(direction == Direction::Up)
            {
                direction = Direction::Right;
            }
            else if(direction == Direction::Right)
            {
                direction = Direction::Down;
            }
            else if(direction == Direction::Down)
            {
                direction = Direction::Left;
            }
            else if(direction == Direction::Left)
            {
                direction = Direction::Up;
            }
        }

        bool obstructed()
        {
            const auto nextPos = nextPosition();
            const auto potentialObstruction = labMap[nextPos.y][nextPos.x];
            return potentialObstruction == '#';
        }

        bool leavingArea()
        {
            const auto leavingLeft = position.x == 0 && direction == Direction::Left;
            const auto leavingRight = position.x == xLabSize-1 && direction == Direction::Right;
            const auto leavingUp = position.y == 0 && direction == Direction::Up;
            const auto leavingDown = position.y == yLabSize-1 && direction == Direction::Down;
            return leavingLeft || leavingRight || leavingUp || leavingDown;
        }

        void printLabMap()
        {
            for(auto l : labMap)
            {
                cout << l << "\n";
            }
            cout << endl;
        }
    };

    Guard guard(lines);
    cout << guard.distinctPositions() << endl;
}
