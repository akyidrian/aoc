#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <regex>
#include <sstream>
#include <unordered_set>

using namespace std;

struct Vector2D
{
    long x;
    long y;

    bool operator==(const Vector2D& other) const {
        return y == other.y && x == other.x;
    }

    struct Hash {
        std::size_t operator()(const Vector2D& v) const {
            return hash<int>()(v.y) ^ (hash<int>()(v.x) << 1);
        }
    };
};
using PositionSet = unordered_set<Vector2D, Vector2D::Hash>;

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

void printRobots(const vector<Robot>& robots, const Vector2D& dimensions)
{
    vector<string> outsideBathroom(dimensions.y, string(dimensions.x, '.'));
    for(const auto& r : robots)
    {
        const auto currentChar = outsideBathroom[r.position.y][r.position.x];
        if(currentChar == '.')
        {
            outsideBathroom[r.position.y][r.position.x] = '1';
        }
        else if(currentChar >= '1' && currentChar < '9')
        {
            outsideBathroom[r.position.y][r.position.x] += 1;
        }
        else if(currentChar == '9')
        {
            outsideBathroom[r.position.y][r.position.x] = 'X';
        }
    }
    for(const auto& l : outsideBathroom)
    {
        cout << l << '\n';
    }
    cout << endl;
}

auto calculateTotalSafetyFactor(vector<Robot> robots, const Vector2D& dimensions, unsigned long deltaTime)
{
    array<unsigned long, 4> quadrantSafetyFactor{};
    for(auto& r : robots)
    {
        r.update(deltaTime, dimensions);

        // Counts the quadrant the robots land in...
        const auto left = r.position.x < (dimensions.x / 2);
        const auto right = r.position.x > (dimensions.x / 2);
        const auto top = r.position.y < (dimensions.y / 2);
        const auto bottom = r.position.y > (dimensions.y / 2);
        if(top && left)
        {
            quadrantSafetyFactor[0]++;
        }
        else if(top && right)
        {
            quadrantSafetyFactor[1]++;
        }
        else if(bottom && left)
        {
            quadrantSafetyFactor[2]++;
        }
        else if(bottom && right)
        {
            quadrantSafetyFactor[3]++;
        }
    }

    return accumulate(quadrantSafetyFactor.begin(), quadrantSafetyFactor.end(), 1ul, [](auto product, auto factor){ return product * factor; });
}

auto findEasterEgg(vector<Robot> robots, const Vector2D dimensions, unsigned long maxDeltaTime = 1000000)
{
    constexpr auto DeltaTimeIncrement = 1ul; // Seconds
    auto deltaTime = DeltaTimeIncrement;
    while(deltaTime <= maxDeltaTime)
    {
        PositionSet positionSet;
        for(auto& r : robots)
        {
            r.update(1/*Second*/, dimensions);
            positionSet.emplace(r.position);
        }

        // Every robot has it's own unique position?
        if(positionSet.size() == robots.size())
        {
            printRobots(robots, dimensions);
            return deltaTime;
        }
        deltaTime += DeltaTimeIncrement;
    }
    return deltaTime;
}

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
            robots.emplace_back(position, velocity);
        }
    }

    const auto totalSafetyFactor = calculateTotalSafetyFactor(robots, dimensions, 100/*Seconds*/);
    const auto elapsedTime = findEasterEgg(robots, dimensions);
    cout << totalSafetyFactor << endl;
    cout << elapsedTime << endl;
}
