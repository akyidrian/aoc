#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<pair<int,int>> getDirections(size_t row, size_t rows, size_t col, size_t cols, size_t strLen)
{
    const auto left = (col+1) >= strLen;
    const auto right = col + strLen <= cols;
    const auto up = (row+1) >= strLen;
    const auto down = row + strLen <= rows;
    vector<pair<int,int>> directions;
    if(left)
    {
        directions.push_back({0, -1});
        if(up)
        {
            directions.push_back({-1, -1});
        }
        if(down)
        {
            directions.push_back({1, -1});
        }
    }
    if(right)
    {
        directions.push_back({0, 1});
        if(up)
        {
            directions.push_back({-1, 1});
        }
        if(down)
        {
            directions.push_back({1, 1});
        }
    }
    if(up)
    {
        directions.push_back({-1, 0});
    }
    if(down)
    {
        directions.push_back({1, 0});
    }
    return directions;
}

unsigned int part1(size_t row, size_t col, const vector<string>& lines)
{
    constexpr char XMAS[] = "XMAS";
    constexpr int XMASLen = sizeof(XMAS)-1;
    const auto center = lines[row][col];
    if(center != XMAS[0])
    {
        return 0;
    }
    const auto rows = lines.size();
    const auto cols = lines[0].size();
    auto directions = getDirections(row, rows, col, cols, XMASLen);

    auto count = 0u;
    for (const auto& [dr, dc] : directions)
    {
        bool matches = true;
        for (int i = 1; i < XMASLen; ++i)
        {
            int rrr = row + i * dr;
            int ccc = col + i * dc;
            if (lines[rrr][ccc] != XMAS[i])
            {
                matches = false;
                break;
            }
        }
        count += matches;
    }
    return count;
}

unsigned int part2(size_t row, size_t col, const vector<string>& lines)
{
    const auto rows = lines.size();
    const auto cols = lines[0].size();

    const auto left = col > 0;
    const auto right = col+1 < cols;
    const auto up = row > 0;
    const auto down = row+1 < rows;

    char center = lines[row][col];
    auto noSpace = !left || !right || !up || !down;
    if(center != 'A' || noSpace)
    {
        return 0;
    }

    auto nw = lines[row-1][col-1];
    auto se = lines[row+1][col+1];
    auto sw = lines[row+1][col-1];
    auto ne = lines[row-1][col+1];
    auto downwardsDiagonal = (nw == 'M' && se == 'S') || (nw == 'S' && se == 'M');
    auto upwardsDiagonal = (sw == 'M' && ne == 'S') || (sw == 'S' && ne == 'M');
    return downwardsDiagonal && upwardsDiagonal;
}

int main(int argc, char** argv)
{
    stringstream example(
    R"(MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX
)");
    ifstream file("../day4.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> lines;
    while (getline(input, line))
    {
        lines.push_back(line);
    }

    auto countPart1 = 0u;
    auto countPart2 = 0u;
    for(auto row = 0u; row < lines.size(); ++row)
    {
        for(auto col = 0u; col < lines[0].size(); ++col)
        {
            countPart1 += part1(row, col, lines);
            countPart2 += part2(row, col, lines);
        }
    }
    cout << "Total count (part 1): " << countPart1 << "\n";
    cout << "Total count (part 2): " << countPart2 << "\n";
}
