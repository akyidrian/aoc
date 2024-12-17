#include <cmath>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <stdexcept>

using namespace std;

auto registerA = 0ul, registerB = 0ul, registerC = 0ul;
vector<unsigned long> program;
auto instructionPointer = 0ul;
stringstream out;

auto combo(unsigned long operand)
{
    switch(operand)
    {
        case(0):
        case(1):
        case(2):
        case(3):
            return operand;
        case(4):
            return registerA;
        case(5):
            return registerB;
        case(6):
            return registerC;
        case(7):
            throw runtime_error("Reserved and will not appear in valid programs");
    }
    return 0ul;
}

void instruction(unsigned long op, unsigned long operand)
{
    switch(op)
    {
        case(0): // adv
        {
            registerA /= pow(2, combo(operand));
            instructionPointer += 2ul;
            return;
        }
        case(1): // bxl
        {
            registerB ^= operand;
            instructionPointer += 2ul;
            return;
        }
        case(2): // bst
        {
            registerB = combo(operand) % 8;
            instructionPointer += 2ul;
            return;
        }
        case(3): // jnz
        {
            if(!registerA)
            {
                instructionPointer += 2ul;
                return;
            }
            instructionPointer = operand;
            return;
        }
        case(4): // bxc
        {
            // Ignore operand (legacy reasons)
            registerB ^= registerC;
            instructionPointer += 2ul;
            return;
        }
        case(5): // out
        {
            out << combo(operand) % 8 << ",";
            instructionPointer += 2ul;
            return;
        }
        case(6): // bdv
        {
            registerB = registerA / pow(2, combo(operand));
            instructionPointer += 2ul;
            return;
        }
        case(7): // cdv
        {
            registerC = registerA / pow(2, combo(operand));
            instructionPointer += 2ul;
            return;
        }
    }
}

int main(int argc, char** argv)
{
    stringstream exampleP1(
R"(Register A: 729
Register B: 0
Register C: 0

Program: 0,1,5,4,3,0)");

    stringstream exampleP2(
R"(Register A: 2024
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0)");

    ifstream file("../day17.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = exampleP2;
    string line;
    regex registerRegex(R"(Register\s([A-C]):\s(\d+))");
    regex programRegex(R"(Program:\s((\d+,?)+))");
    smatch match;
    getline(input, line);
    if(regex_match(line, match, registerRegex))
    {
        registerA = stoul(match[2]);
    }
    getline(input, line);
    if(regex_match(line, match, registerRegex))
    {
        registerB = stoul(match[2]);
    }
    getline(input, line);
    if(regex_match(line, match, registerRegex))
    {
        registerC = stoul(match[2]);
    }
    getline(input, line); // \n
    getline(input, line);
    string programStr;
    if(regex_match(line, match, programRegex))
    {
        programStr = match[1].str();
        for (auto&& range : programStr | views::split(','))
        {
            string_view sv(&*range.begin(), ranges::distance(range));
            program.emplace_back(stoul(string(sv)));
        }
    }
    cout << registerA << endl;
    cout << registerB << endl;
    cout << registerC << endl;
    for(const auto& p : program)
    {
        cout << p << ",";
    }
    cout << endl;
    cout << programStr << endl;

    for(auto it = program.begin(); it != program.end();)
    {
        auto op = *it;
        auto operand = *(it+1);
        instruction(op, operand);
        it = program.begin() + instructionPointer;
    }
    cout << out.str() << endl;

}
