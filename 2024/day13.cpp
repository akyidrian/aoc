#include <fstream>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>

using namespace std;

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

    struct Axis 
    {
        unsigned int x;
        unsigned int y;
    };
    vector<Axis> buttonA;
    vector<Axis> buttonB;
    vector<Axis> prize;
    for(const auto& l : lines)
    {
        if(l.empty())
        {
            continue;
        }
        if(l.substr(0, 6) == "Button")
        {
            regex buttonTokenRegex(R"(Button\s([A-B]):\sX\+(\d+),\sY\+(\d+))");
            smatch match;
            if(regex_match(l, match, buttonTokenRegex))
            {
                if(match[1] == 'A')
                {
                    buttonA.emplace_back(stoul(match[2]), stoul(match[3]));
                }
                else if(match[1] == 'B')
                {
                    buttonB.emplace_back(stoul(match[2]), stoul(match[3]));
                }
                else
                {
                    cerr << "Failed to parse button" << endl;
                    return 1;
                }
            }
        }
        else if(l.substr(0, 5) == "Prize")
        {
            regex prizeTokenRegex(R"(Prize:\sX=(\d+),\sY=(\d+))");
            smatch match;
            if(regex_match(l, match, prizeTokenRegex))
            {
                prize.emplace_back(stoul(match[1]), stoul(match[2]));
            }
        }
        else
        {
            cerr << "Failed to detect 'Button' or 'Prize' substring during parsing" << endl;
            return 1;
        }
    }

    constexpr auto ButtonPressLimit = 100u;
    auto totalTokens = 0u;
    for(auto n = 0u; n < buttonA.size(); ++n)
    {
        const auto a = buttonA[n];
        const auto b = buttonB[n];
        const auto p = prize[n];
        vector<pair<unsigned long, unsigned long>> pressed;
        for(auto i = 0u; i < ButtonPressLimit; ++i)
        {
            for(auto j = 0u; j < ButtonPressLimit; ++j)
            {
                if((a.x*i + b.x*j == p.x) && (a.y*i + b.y*j == p.y))
                {
                    pressed.emplace_back(i, j);
                }
            }
        }

        if(!pressed.size())
        {
            continue;
        }

        auto tokens = 3*pressed[0].first + pressed[0].second;
        for(const auto& [i, j] : pressed)
        {
            cout << i << ", " << j << endl;
            const auto newTokens = 3 * i + j;
            if(newTokens < tokens)
            {
                tokens = newTokens;
            }
        }
        totalTokens += tokens;
    }
    cout << totalTokens << endl;
}
