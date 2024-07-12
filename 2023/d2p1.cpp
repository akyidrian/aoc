#include "helper.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <regex>
#include <vector>

using std::cout;
using std::ostream;
using std::smatch;
using std::string;
using std::stringstream;
using std::string_view;
using std::to_string;
using std::regex;
using std::sregex_iterator;
using std::vector;

struct Cubes
{
    unsigned int red = 0u;
    unsigned int green = 0u;
    unsigned int blue = 0u;
};

class Game
{
    public:
        Game(const unsigned int n) : m_number(n)
        {
        }

        void add(const Cubes& round)
        {
            m_rounds.push_back(round);
        }

        // Is the game possible if the given bag is used?
        bool possible(const Cubes& bag) const
        {
            for(const auto& cubes : m_rounds)
            {
                if(cubes.red > bag.red || cubes.green > bag.green || cubes.blue > bag.blue)
                {
                    return false;
                }
            }
            return true;
        }

        unsigned int number() const
        {
            return m_number;
        }

        // What are the fewest number of cubes required for the game to be possible?
        Cubes fewest() const
        {
            auto red = 0u;
            auto green = 0u;
            auto blue = 0u;
            for(const auto& r : m_rounds)
            {
                if(r.red > red)
                {
                    red = r.red;
                }
                if(r.green > green)
                {
                    green = r.green;
                }
                if(r.blue > blue)
                {
                    blue = r.blue;
                }
            }
            return {red, green, blue};
        }

        // Created for debug purposes...
        friend ostream& operator<<(ostream& out, const Game& game)
        {
            stringstream ss;
            auto rounds = game.m_rounds;
            for(auto i = 0u; i < rounds.size(); ++i)
            {
                ss << rounds[i].red << " red, ";
                ss << rounds[i].green << " green, ";
                ss << rounds[i].blue << " blue; ";
            }
            return out << string("Game ") + to_string(game.m_number) + string(": ") + ss.str();
        }
    private:
        unsigned int m_number;
        vector<Cubes> m_rounds;
};

Game parseGameRecord(const string& record)
{
    const auto colonPos = record.find(':');
    constexpr string_view gameSubstr{"Game "};
    const auto gameNumber = static_cast<unsigned int>(stoul(record.substr(gameSubstr.size(), colonPos-gameSubstr.size())));
    auto game = Game(gameNumber);

    regex roundPattern("(\\d+ (?:red|green|blue)(?:, \\d+ (?:red|green|blue))*)");
    sregex_iterator roundIt(record.begin() + static_cast<long>(colonPos), record.end(), roundPattern);
    sregex_iterator endIt;
    while (roundIt != endIt) {
        smatch roundMatch = *roundIt;
        const string roundMatchStr = roundMatch.str(0);

        regex cubeColoursPattern("(\\d+) (red|green|blue)");
        sregex_iterator coloursIt(roundMatchStr.begin(), roundMatchStr.end(), cubeColoursPattern);
        auto red = 0ul;
        auto green = 0ul;
        auto blue = 0ul;
        while (coloursIt != endIt) {
            smatch coloursMatch = *coloursIt;
            if(coloursMatch.str(2) == "red")
            {
                red = stoul(coloursMatch.str(1));
            }
            else if(coloursMatch.str(2) == "green")
            {
                green = stoul(coloursMatch.str(1));
            }
            else if(coloursMatch.str(2) == "blue")
            {
                blue = stoul(coloursMatch.str(1));
            }
            ++coloursIt;
        }
        game.add({static_cast<unsigned int>(red), static_cast<unsigned int>(green), static_cast<unsigned int>(blue)});

        ++roundIt;
    }

    return game;
}

vector<Game> parseGameRecords(const vector<string>& records)
{
    vector<Game> games;
    games.reserve(records.size());
    for(const auto& r : records)
    {
        games.push_back(parseGameRecord(r));
    }
    return games;
}

[[nodiscard]] unsigned int runPart1(const vector<string>& records)
{
    constexpr static Cubes bag{12, 13, 14}; // Same for all games...
    vector<Game> games = parseGameRecords(records);
    auto sum = 0u;
    for(const auto& g : games)
    {
        if(g.possible(bag))
        {
            sum += g.number();
        }
    }
    return sum;
}

[[nodiscard]] unsigned int runPart2(const vector<string>& records)
{
    vector<Game> games = parseGameRecords(records);
    auto sum = 0u;
    for(const auto& g : games)
    {
        const auto fewest = g.fewest();
        sum += fewest.red * fewest.green * fewest.blue; // "power of minimum set of cubes"
    }
    return sum;
}

int main(int argc, char** argv)
{
    using namespace helper;

    const auto [part, filePath] = parseMainArgs(argc, argv);

    // Common example for both parts
    vector<string> example{
        "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green",
        "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue",
        "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red",
        "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red",
        "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"
    };
    vector<string>& records = example;
    if(!filePath.empty())
    {
        records = readFile(filePath);
    }

    switch(part)
    {
        case Part::One:
        {
            constexpr auto exampleAnswer = 8u;
            constexpr auto inputAnswer = 2776u;
            const auto expected = filePath.empty() ? exampleAnswer : inputAnswer;
            auto answer = runPart1(records);
            cout << "Sum of possible game numbers: " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
        case Part::Two:
        {
            constexpr auto exampleAnswer = 2286u;
            constexpr auto inputAnswer = 68638u;
            const auto expected = filePath.empty() ? exampleAnswer : inputAnswer;
            auto answer = runPart2(records);
            cout << "Sum of powers of minimum set of cubes: " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
    }

    return 0;
}
