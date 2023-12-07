#include "helper.h"

#include <cmath>
#include <iostream>
#include <numeric>
#include <set>
#include <stack>
#include <string>
#include <regex>
#include <vector>

using std::cerr;
using std::cout;
using std::set;
using std::smatch;
using std::string;
using std::string_view;
using std::sregex_iterator;
using std::regex;
using std::vector;

vector<unsigned int> parseCards(const vector<string>& cards)
{
    vector<unsigned int> matchedCounts;
    matchedCounts.reserve(cards.size());
    for(const auto& card : cards)
    {
        const auto colonPos = card.find(':');
        constexpr string_view cardSubstr{"Card "};
        const auto cardNumber = static_cast<unsigned int>(stoul(card.substr(cardSubstr.size(), colonPos-cardSubstr.size())));
        if(cardNumber > cards.size())
        {
            cerr << "Invalid card number. Expected card numbers to be less than or equal to the number of cards provided..." << '\n';
            return {};
        }

        const auto barPos = card.find('|');
        regex numbersPattern("(\\d+)");
        sregex_iterator winningNumbersIt(card.begin() + static_cast<long>(colonPos), card.begin() + static_cast<long>(barPos), numbersPattern);
        const sregex_iterator endIt;
        set<unsigned int> winningNumbers;
        for(; winningNumbersIt != endIt; ++winningNumbersIt)
        {
            smatch winningNumberMatch = *winningNumbersIt;
            const unsigned int winningNumber = static_cast<unsigned int>(stoul(winningNumberMatch.str(1)));
            winningNumbers.insert(winningNumber);
        }

        sregex_iterator numbersGivenIt(card.begin() + static_cast<long>(barPos), card.end(), numbersPattern);
        vector<unsigned int> numbersGiven;
        for(; numbersGivenIt != endIt; ++numbersGivenIt)
        {
            smatch numberGivenMatch = *numbersGivenIt;
            unsigned int numberGiven = static_cast<unsigned int>(stoul(numberGivenMatch.str(1)));
            numbersGiven.push_back(numberGiven);
        }

        const auto count = count_if(numbersGiven.begin(), numbersGiven.end(), [&winningNumbers](auto n){ return winningNumbers.contains(n); });
        matchedCounts.insert(matchedCounts.begin() + (cardNumber-1), static_cast<unsigned int>(count));
    }
    return matchedCounts;
}

[[nodiscard]] auto runPart1(const vector<string>& cards)
{
    const auto matchedCounts = parseCards(cards);

    auto points = 0u;
    for(const auto& c : matchedCounts)
    { 
        // Relying on integer rounding for c == 0
        // I.e. 2^-1 = 0.5 -> 0
        points += static_cast<unsigned int>(pow(2, c-1));
    }
    return points; 
}

[[nodiscard]] auto runPart2(const vector<string>& cards)
{
    const auto matchedCounts = parseCards(cards);
    auto cardCount = 0u;
    std::stack<vector<unsigned int>> unprocessedCards;
    vector<unsigned int> original(matchedCounts.size());
    iota(original.begin(), original.end(), 0);
    unprocessedCards.push(original);
    while(unprocessedCards.size())
    {
        const auto cardIndices = unprocessedCards.top();
        unprocessedCards.pop();
        for(const auto& i : cardIndices)
        {
            cardCount += 1; // count "this" card

            // Push the indices of card copies that need to be processed as a result of "this" card
            const auto cardsRemaining = matchedCounts.size() - (i+1);
            size_t count = 0u;
            if(matchedCounts[i] <= cardsRemaining)
            {
                count = matchedCounts[i];
            }
            else 
            {
                count = cardsRemaining;        
            }
            vector<unsigned int> cardCopies(count);
            iota(cardCopies.begin(), cardCopies.end(), i+1);
            unprocessedCards.push(cardCopies);
        }
    }
    return cardCount;
}

int main(int argc, char** argv)
{
    using namespace helper;

    const auto [part, filePath] = parseMainArgs(argc, argv);

    // Same example for both parts
    vector<string> example{
        "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53",
        "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19",
        "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1",
        "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83",
        "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36",
        "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"
    };
    vector<string>& cards = example;
    if(!filePath.empty())
    {
        cards = readFile(filePath);
    }
    switch(part)
    {
        case Part::One:
        {
            constexpr auto exampleAnswer = 13u;
            constexpr auto inputAnswer = 23678u;
            const auto expected = filePath.empty() ? exampleAnswer : inputAnswer;
            const auto answer = runPart1(cards);
            cout << "Sum of points: " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
        case Part::Two:
        {
            constexpr auto exampleAnswer = 30u;
            constexpr auto inputAnswer = 15455663u;
            const auto expected = filePath.empty() ? exampleAnswer : inputAnswer;
            const auto answer = runPart2(cards);
            cout << "Number of scratch cards: " << answer << '\n';
            cout << "Correct? " << (answer == expected) << '\n';
            break;
        }
    }

    return 0;
}
