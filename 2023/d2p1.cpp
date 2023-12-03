#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <regex>
#include <vector>

using std::cerr;
using std::cout;
using std::ifstream;
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
    unsigned int red = 0;
    unsigned int green = 0;
    unsigned int blue = 0;
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

        bool possible() const
        {
            for(const auto& cubes : m_rounds)
            {
                if(cubes.red > m_bag.red || cubes.green > m_bag.green || cubes.blue > m_bag.blue)
                {
                    return false;
                }
            }
            return true;
        }

        bool impossible()
        {
            return !possible();
        }

        unsigned int number() const
        {
            return m_number;
        }

        friend ostream& operator<<(ostream& out, const Game& game)
        {
            stringstream ss;
            auto rounds = game.m_rounds;
            for(auto i = 0u; i < rounds.size(); ++i)  // TODO use fmt - it's just easier
            {
                if(rounds[i].red)
                {
                    ss << rounds[i].red << " red, ";
                }
                if(rounds[i].green)
                {
                    ss << rounds[i].green << " green, ";
                }
                if(rounds[i].blue)
                {
                    ss << rounds[i].blue << " blue, ";
                }
                ss << "; ";
            }
            return out << string("Game ") + to_string(game.m_number) + string(": ") + ss.str();
        }
    private:
        vector<Cubes> m_rounds;
        unsigned int m_number;
        constexpr static Cubes m_bag{12, 13, 14};
};

vector<string> readGameRecord(const string& filePath)
{
    ifstream inputFile(filePath);
    if(!inputFile.is_open())
    {
        cerr << "Error opening input file: " << filePath << '\n';
        return {};
    }
    vector<string> input;
    string line;
    while(getline(inputFile, line))
    {
        input.push_back(line);
    }
    return input;
}

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

int main(int argc, char** argv)
{
    vector<string> example{
        "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green",
        "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue",
        "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red",
        "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red",
        "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green"
    };
    vector<string>& records = example;

    const auto useExample = argc == 1;
    if(!useExample)
    {
        records = readGameRecord(argv[1]);
    }

    vector<Game> games = parseGameRecords(records);
    auto sum = 0u;
    for(const auto& g : games)
    {
        if(g.possible())
        {
            sum += g.number();
        }
    }
    cout << "Sum of possible game numbers: " << sum << '\n';

    return 0;
}
