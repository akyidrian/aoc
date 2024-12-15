#include <array>
#include <fstream>
#include <iostream>
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

    // TODO: This was wrong in other code...
    void operator+=(const Vector2D& other)
    {
        y += other.y;
        x += other.x;
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
constexpr array<Direction, 4> allDirections{Direction::Up, Direction::Down, Direction:: Left, Direction::Right};
using direction_t = std::underlying_type_t<Direction>;

constexpr array<Vector2D, 4> directionMap{{/*Up*/{-1, 0}, /*Down*/ {1, 0}, /*Left*/ {0, -1}, /*Right*/ {0, 1}}};

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

int main(int argc, char** argv)
{
    stringstream example1(
R"(########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

<^^>>>vv<v>>v<<)");

    stringstream example2(
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

    enum class Parsing
    {
        WarehouseMap,
        MovementAttempts
    };
    ifstream file("../day15.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> warehouseMap;
    string movementAttempts;
    auto parsingMode = Parsing::WarehouseMap;
    while (getline(input, line)) {
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

    const auto yWarehouseMapSize = warehouseMap.size();
    const auto xWarehouseMapSize = warehouseMap[0].size();

    // Find robot location...
    Vector2D robot;
    for(auto y = 0; y < yWarehouseMapSize; ++y)
    {
        for(auto x = 0; x < xWarehouseMapSize; ++x)
        {
            if(warehouseMap[y][x] == '@')
            {
                robot = {y, x};
            }
        }
    }

    printWarehouseMap(warehouseMap);
    unordered_map<char, uint8_t> charToDirMap{{'^', 0}, {'v', 1}, {'<', 2}, {'>', 3}};
    for(const auto& dirChar : movementAttempts)
    {
        //cout << dirChar << endl;
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
    cout << sum << endl;
}
