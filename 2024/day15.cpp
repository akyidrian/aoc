#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Vector2D
{
    int y;
    int x;

    Vector2D operator+(const Vector2D& other) const
    {
        return Vector2D{.y = y + other.y, .x = x + other.x};
    }

    void operator+=(const Vector2D& other)
    {
        y += other.y;
        x += other.x;
    }

    Vector2D operator-(const Vector2D& other)
    {
        return Vector2D{.y = y - other.y, .x = x - other.x};
    }

    void operator-=(const Vector2D& other)
    {
        y -= other.y;
        x -= other.x;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v)
    {
        os << "(" << v.y << ", " << v.x << ")";
        return os;
    }
};

enum class Direction : uint8_t
{
    Up = 0,
    Down,
    Left,
    Right
};
using direction_t = std::underlying_type_t<Direction>;

constexpr array<Direction, 4> allDirections{Direction::Up, Direction::Down, Direction:: Left, Direction::Right};
constexpr array<Vector2D, 4> directionMap{{/*Up*/{-1, 0}, /*Down*/ {1, 0}, /*Left*/ {0, -1}, /*Right*/ {0, 1}}};
unordered_map<char, uint8_t> charToDirMap{{'^', 0}, {'v', 1}, {'<', 2}, {'>', 3}};

void printWarehouseMap(const vector<string>& warehouseMap, bool skip = true)
{
    if(skip)
    {
        return;
    }
    for(const auto& l : warehouseMap)
    {
        cout << l << '\n';
    }
    cout << endl;
    cin.get();
}

Vector2D findRobot(const vector<string>& warehouseMap)
{
    for(auto y = 0; y < warehouseMap.size(); ++y)
    {
        for(auto x = 0; x < warehouseMap[0].size(); ++x)
        {
            if(warehouseMap[y][x] == '@')
            {
                return {y, x};
            }
        }
    }
    return {};
}

bool canMove(vector<string>& warehouseMap, const Vector2D& pos, const Vector2D& dir)
{
    const Vector2D next = pos + dir;
    auto ret = true;
    if(warehouseMap[next.y][next.x] == '#')
    {
        return false;
    }
    else if(warehouseMap[next.y][next.x] == '.')
    {
        return true;
    }
    if(warehouseMap[next.y][next.x] == '[')
    {
        Vector2D besideNext = next + directionMap[static_cast<size_t>(Direction::Right)];
        ret = ret && canMove(warehouseMap, next, dir);
        ret = ret && canMove(warehouseMap, besideNext, dir);
    }
    else
    {
        Vector2D besideNext = next + directionMap[static_cast<size_t>(Direction::Left)];
        ret = ret && canMove(warehouseMap, next, dir);
        ret = ret && canMove(warehouseMap, besideNext, dir);
    }
    return ret;
}

void doMove(vector<string>& warehouseMap, const Vector2D& pos, const Vector2D& dir)
{
    const Vector2D next = pos + dir;
    if(warehouseMap[next.y][next.x] == '.')
    {
        warehouseMap[next.y][next.x] = warehouseMap[pos.y][pos.x];
        warehouseMap[pos.y][pos.x] = '.';
        return;
    }
    Vector2D beside;
    Vector2D besideNext;
    if(warehouseMap[next.y][next.x] == '[')
    {
        Vector2D beside = pos + directionMap[static_cast<size_t>(Direction::Right)];
        Vector2D besideNext = next + directionMap[static_cast<size_t>(Direction::Right)];
    }
    else if(warehouseMap[next.y][next.x] == ']')
    {
        Vector2D beside = pos + directionMap[static_cast<size_t>(Direction::Left)];
        Vector2D besideNext = next + directionMap[static_cast<size_t>(Direction::Left)];
    }
    doMove(warehouseMap, next, dir);
    doMove(warehouseMap, besideNext, dir);
    warehouseMap[next.y][next.x] = warehouseMap[pos.y][pos.x];
    warehouseMap[pos.y][pos.x] = '.';
}

auto part1(vector<string> warehouseMap, const string& movementAttempts)
{
    printWarehouseMap(warehouseMap);
    const auto yWarehouseMapSize = warehouseMap.size();
    const auto xWarehouseMapSize = warehouseMap[0].size();
    auto robot = findRobot(warehouseMap);

    for(const auto& dirChar : movementAttempts)
    {
        const auto dir =  directionMap[charToDirMap[dirChar]];
        auto next = robot + dir;
        if(warehouseMap[next.y][next.x] == '.')
        {
            warehouseMap[robot.y][robot.x] = '.';
            robot += dir;
            warehouseMap[robot.y][robot.x] = '@';
            printWarehouseMap(warehouseMap);
            continue;
        }
        while(warehouseMap[next.y][next.x] == 'O')
        {
            next += dir;
        }
        if(warehouseMap[next.y][next.x] == '#')
        {
            printWarehouseMap(warehouseMap);
            continue;
        }
        warehouseMap[next.y][next.x] = 'O';
        warehouseMap[robot.y][robot.x] = '.';
        robot += dir;
        warehouseMap[robot.y][robot.x] = '@';
        printWarehouseMap(warehouseMap);
    }

    auto sum = 0ul;
    for(auto y = 0u; y < yWarehouseMapSize; ++y)
    {
        for(auto x = 0u; x < xWarehouseMapSize; ++x)
        {
            sum += (warehouseMap[y][x] == 'O') ? 100*y + x : 0ul;
        }
    }
    return sum;
}

void scaleUp(vector<string>& warehouseMap)
{
    vector<string> newWarehouseMap;
    for(auto y = 0; y < warehouseMap.size(); ++y)
    {
        stringstream ss;
        for(auto x = 0; x < warehouseMap[0].size(); ++x)
        {
            auto tile = warehouseMap[y][x];
            if(tile == '.')
            {
                ss << "..";
            }
            else if(tile == '#')
            {
                ss << "##";
            }
            else if(tile == 'O')
            {
                ss << "[]";
            }
            else if(tile == '@')
            {
                ss << "@.";
            }
        }
        newWarehouseMap.emplace_back(ss.str());
    }
    warehouseMap = newWarehouseMap;
}

auto part2(vector<string> warehouseMap, const string& movementAttempts)
{
    scaleUp(warehouseMap);
    const auto yWarehouseMapSize = warehouseMap.size();
    const auto xWarehouseMapSize = warehouseMap[0].size();
    auto robot = findRobot(warehouseMap);

    printWarehouseMap(warehouseMap);
    for(const auto& dirChar : movementAttempts)
    {
        const auto dir =  directionMap[charToDirMap[dirChar]];
        auto next = robot + dir;
        if(warehouseMap[next.y][next.x] == '.')
        {
            warehouseMap[robot.y][robot.x] = '.';
            robot += dir;
            warehouseMap[robot.y][robot.x] = '@';
            printWarehouseMap(warehouseMap);
            continue;
        }
        else if(warehouseMap[next.y][next.x] == '#')
        {
            printWarehouseMap(warehouseMap);
            continue;
        }

        if(dirChar == '<') // Must be hitting ']'
        {
            while(warehouseMap[next.y][next.x] == ']')
            {
                next += dir + dir; // Jump over '['
            }
            if(warehouseMap[next.y][next.x] == '#')
            {
                printWarehouseMap(warehouseMap);
                continue;
            }
            string boxes(warehouseMap[next.y].begin()+next.x+1, warehouseMap[next.y].begin()+robot.x);
            copy(boxes.begin(), boxes.end(), warehouseMap[next.y].begin()+next.x);
            warehouseMap[robot.y][robot.x] = '.';
            robot += dir;
            warehouseMap[robot.y][robot.x] = '@';
            printWarehouseMap(warehouseMap);
            continue;
        }
        else if(dirChar == '>') // Must be hitting ']'
        {
            while(warehouseMap[next.y][next.x] == '[')
            {
                next += dir + dir; // Jump over ']'
            }
            if(warehouseMap[next.y][next.x] == '#')
            {
                printWarehouseMap(warehouseMap);
                continue;
            }
            string boxes(warehouseMap[next.y].begin()+robot.x+1, warehouseMap[next.y].begin()+next.x);
            copy(boxes.begin(), boxes.end(), warehouseMap[next.y].begin()+robot.x+2);
            warehouseMap[robot.y][robot.x] = '.';
            robot += dir;
            warehouseMap[robot.y][robot.x] = '@';
            printWarehouseMap(warehouseMap);
            continue;
        }
        else if(dirChar == '^' || dirChar == 'v')
        {
            if(canMove(warehouseMap, robot, dir))
            {
                doMove(warehouseMap, robot, dir);
                robot += dir;
            }
            printWarehouseMap(warehouseMap);
            continue;
        }
    }

    auto sum = 0ul;
    for(auto y = 0u; y < yWarehouseMapSize; ++y)
    {
        for(auto x = 0u; x < xWarehouseMapSize; ++x)
        {
            sum += (warehouseMap[y][x] == '[') ? 100*y + x : 0ul;
        }
    }
    return sum;
}

int main(int argc, char** argv)
{
    stringstream exampleSmallP1(
R"(########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

<^^>>>vv<v>>v<<)");

    stringstream exampleLarge(
R"(##########
#..O..O.O#
#......O.#
#.OO..O.O#
#..O@..O.#
#O#..O...#
#O..O..O.#
#.OO.O.OO#
#....O...#
##########

<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^)");

    stringstream exampleSmallP2(
R"(#######
#...#.#
#.....#
#..OO@#
#..O..#
#.....#
#######

<vv<<^^<<^^)");

    ifstream file("../day15.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }

    enum class Parsing
    {
        WarehouseMap,
        MovementAttempts
    };
    auto parsingMode = Parsing::WarehouseMap;
    auto& input = file;
    string line;
    vector<string> warehouseMap;
    string movementAttempts;
    while (getline(input, line))
    {
        if(line == "")
        {
            parsingMode = Parsing::MovementAttempts;
            continue;
        }
        if(parsingMode == Parsing::WarehouseMap)
        {
            warehouseMap.emplace_back(line);
        }
        else if(parsingMode == Parsing::MovementAttempts)
        {
            movementAttempts += line;
        }
    }

    const auto sumP1 = part1(warehouseMap, movementAttempts);
    const auto sumP2 = part2(warehouseMap, movementAttempts);
    cout << sumP1 << endl;
    cout << sumP2 << endl;
}
