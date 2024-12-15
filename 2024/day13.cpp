#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

using namespace std;

struct Axis
{
    double x;
    double y;

    Axis operator+(const double scalar) const
    {
        return {.x = x + scalar, .y = y + scalar};
    }
};

struct Machines
{
    vector<Axis> buttonA;
    vector<Axis> buttonB;
    vector<Axis> prize;
};

bool parse(Machines& machines, const vector<string>& lines)
{
    for(const auto& l : lines)
    {
        if(l.empty())
        {
            continue;
        }
        if(l.substr(0, 6) == "Button")
        {
            regex buttonRegex(R"(Button\s([A-B]):\sX\+(\d+),\sY\+(\d+))");
            smatch match;
            if(regex_match(l, match, buttonRegex))
            {
                if(match[1] == 'A')
                {
                    machines.buttonA.emplace_back(stoul(match[2]), stoul(match[3]));
                }
                else if(match[1] == 'B')
                {
                    machines.buttonB.emplace_back(stoul(match[2]), stoul(match[3]));
                }
                else
                {
                    cerr << "Failed to match on button 'A' or button 'B'" << endl;
                    return false;
                }
            }
            else
            {
                cerr << "Failed to match on button regex" << endl;
                return false;
            }
        }
        else if(l.substr(0, 5) == "Prize")
        {
            regex prizeRegex(R"(Prize:\sX=(\d+),\sY=(\d+))");
            smatch match;
            if(regex_match(l, match, prizeRegex))
            {
                machines.prize.emplace_back(stoul(match[1]), stoul(match[2]));
            }
            else
            {
                cerr << "Failed to match on prize regex" << endl;
                return false;
            }
        }
        else
        {
            cerr << "Failed to detect 'Button' or 'Prize' substring" << endl;
            return false;
        }
    }
    return true;
}

auto calculateTotalTokens(const Machines& machines, unsigned long conversionError = 0)
{
    auto totalTokens = 0ul;
    for(auto n = 0u; n < machines.buttonA.size(); ++n)
    {
        const auto a = machines.buttonA[n];
        const auto b = machines.buttonB[n];
        const auto p = machines.prize[n] + conversionError;
        const double det = a.x*b.y - b.x*a.y;
        if(det == 0) // No solution
        {
            continue;
        }
        array<double, 4> inverseMat{b.y, -b.x, -a.y, a.x};

        // Using unsigned long conversion to floor the double...
        auto i = static_cast<unsigned long>((inverseMat[0]*p.x + inverseMat[1]*p.y) / det);
        auto j = static_cast<unsigned long>((inverseMat[2]*p.x + inverseMat[3]*p.y) / det);

        // If we can recalculate the prize (after the floor), it's a valid i,j combination...
        if(i >= 0 && j >= 0 && (a.x*i + b.x*j == p.x) && (a.y*i + b.y*j == p.y))
        {
            totalTokens += 3*i + j;
        }
    }
    return totalTokens;
}

int main(int argc, char** argv)
{
    stringstream example(R"(Button A: X+94, Y+34
Button B: X+22, Y+67
Prize: X=8400, Y=5400

Button A: X+26, Y+66
Button B: X+67, Y+21
Prize: X=12748, Y=12176

Button A: X+17, Y+86
Button B: X+84, Y+37
Prize: X=7870, Y=6450

Button A: X+69, Y+23
Button B: X+27, Y+71
Prize: X=18641, Y=10279)");
    ifstream file("../day13.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }

    auto& input = file;
    string line;
    vector<string> lines;
    while (getline(input, line))
    {
        lines.emplace_back(line);
    }

    Machines machines;
    if(!parse(machines, lines))
    {
        cerr << "Failed to parse" << endl;
        return 1;
    }

    const auto totalTokensP1 = calculateTotalTokens(machines);
    const auto totalTokensP2 = calculateTotalTokens(machines, 10000000000000);
    cout << totalTokensP1 << endl;
    cout << totalTokensP2 << endl;
}
