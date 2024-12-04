#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

unsigned int part1(int row, int col, const vector<string>& lines)
{
    constexpr char x[] = "XMAS";
    constexpr int xLen = sizeof(x)-1;
    if(lines[row][col] != x[0])
    {
        return 0;
    }
    const int rows = lines.size();
    const int cols = lines[0].size();

    auto count = 0u;
    const auto left = (col+1)-xLen >= 0;
    const auto right = col + xLen <= cols;
    const auto up = (row+1)-xLen >= 0;
    const auto down = row + xLen <= rows;

    auto i = 1;
    while(left && i<xLen && lines[row][col-i]==x[i])
    {
        i++;
    }
    count += i==xLen;

    i = 1;
    while(right && i<xLen && lines[row][col+i]==x[i])
    {
        i++;
    }
    count += i==xLen;

    i = 1;
    while(up && i<xLen && lines[row-i][col]==x[i])
    {
        i++;
    }
    count += i==xLen;


    i = 1;
    while(down && i<xLen && lines[row+i][col]==x[i])
    {
        i++;
    }
    count += i==xLen;

    i = 1;
    while(left && up && i<xLen && lines[row-i][col-i]==x[i])
    {
        i++;
    }
    count += i==xLen;

    i = 1;
    while(left && down && i<xLen && lines[row+i][col-i]==x[i])
    {
        i++;
    }
    count += i==xLen;

    i = 1;
    while(right && up && i<xLen && lines[row-i][col+i]==x[i])
    {
        i++;
    }
    count += i==xLen;

    i = 1;
    while(right && down && i<xLen && lines[row+i][col+i]==x[i])
    {
        i++;
    }
    count += i==xLen;
    return count;
}

unsigned int part2(int row, int col, const vector<string>& lines)
{
    const int rows = lines.size();
    const int cols = lines[0].size();

    auto count = 0u;
    const auto left = col-1 >= 0;
    const auto right = col+1 < cols;
    const auto up = row-1 >= 0;
    const auto down = row+1 < rows;
    if(lines[row][col] != 'A' || !left || !right || !up || !down)
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
