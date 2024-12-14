#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <regex>
#include <sstream>

using namespace std;

struct Vector2D
{
    long x;
    long y;
};

struct Robot
{
    Vector2D position;
    Vector2D velocity;

    void update(long deltaTime, const Vector2D& dimensions)
    {
        position.x = (position.x + velocity.x * deltaTime) % dimensions.x;
        if(position.x < 0)
        {
            position.x = dimensions.x + position.x;
        }
        position.y = (position.y + velocity.y * deltaTime) % dimensions.y;
        if(position.y < 0)
        {
            position.y = dimensions.y + position.y;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Robot& r)
    {
        os << "Position=" << "(" << r.position.x << "," << r.position.y << ")" <<
            ", Velocity=" << "(" << r.velocity.x << "," << r.velocity.y << ")";
        return os;
    }
};

enum class Input
{
    Example,
    File
};

int main(int argc, char** argv)
{
    const auto exampleWidth = 11;
    const auto exampleHeight = 7;
    string example(R"(p=0,4 v=3,-3
p=6,3 v=-1,-3
p=10,3 v=-1,2
p=2,0 v=2,-1
p=0,0 v=1,3
p=3,0 v=-2,-2
p=7,6 v=-1,-3
p=3,0 v=-1,-2
p=9,3 v=2,3
p=7,3 v=-1,2
p=2,4 v=2,-3
p=9,5 v=-3,-3)");

    const auto fileWidth = 101;
    const auto fileHeight = 103;

    unique_ptr<istream> input = nullptr;
    Vector2D dimensions;
    const auto inputMode = Input::File;
    if(inputMode == Input::Example)
    {
        input = make_unique<stringstream>(example);
        dimensions = {exampleWidth, exampleHeight};
    }
    else if(inputMode == Input::File)
    {
        input = make_unique<ifstream>("../day14.txt");
        if (!*input)
        {
            cerr << "Failed to open file" << "\n";
            return 1;
        }
        dimensions = {fileWidth, fileHeight};
    }
    string line;
    vector<Robot> robots;
    const regex robotRegex(R"(p=(-?\d+),(-?\d+)\sv=(-?\d+),(-?\d+))");
    while (getline(*input, line))
    {
        smatch match;
        if(regex_match(line, match, robotRegex))
        {
            const Vector2D position{stol(match[1]), stol(match[2])};
            const Vector2D velocity{stol(match[3]), stol(match[4])};
            const Robot robot(position, velocity);
            robots.emplace_back(robot);
        }
    }

    for(auto& r : robots)
    {
        r.update(100/*seconds*/, dimensions);
    }

    for(const auto& r : robots)
    {
        cout << r << endl;
    }

    array<unsigned long, 4> quadrantSafetyFactor{};
    for(const auto& r : robots)
    {
        const auto left = r.position.x < (dimensions.x / 2);
        const auto right = r.position.x > (dimensions.x / 2);
        const auto top = r.position.y < (dimensions.y / 2);
        const auto bottom = r.position.y > (dimensions.y / 2);
        if(left && top)
        {
            quadrantSafetyFactor[0]++;
        }
        else if(right && top)
        {
            quadrantSafetyFactor[1]++;
        }
        else if(left && bottom)
        {
            quadrantSafetyFactor[2]++;
        }
        else if(right && bottom)
        {
            quadrantSafetyFactor[3]++;
        }
    }
    for(auto& a : quadrantSafetyFactor)
    {
        cout << a << endl;
    }
    auto totalSafetyFactor = accumulate(quadrantSafetyFactor.begin(), quadrantSafetyFactor.end(), 1ul, [](auto product, auto factor){ return product * factor; });
    cout << totalSafetyFactor << endl;
}
