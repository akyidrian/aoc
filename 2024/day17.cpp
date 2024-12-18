#include <cmath>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <sstream>
#include <stdexcept>

using namespace std;

enum class Opcode : uint8_t
{
    Adv = 0,
    Bxl,
    Bst,
    Jnz,
    Bxc,
    Out,
    Bdv,
    Cdv
};

struct Computer
{
    unsigned long a = 0ul;
    unsigned long b = 0ul;
    unsigned long c = 0ul;
    vector<uint8_t> program;
    long p = 0l; // 'instruction pointer'
    vector<uint8_t> out;

    Computer() = delete;
    Computer(unsigned long a, unsigned long b, unsigned long c, const vector<uint8_t>& program)
    : a(a), b(b), c(c), program(program) {}

    unsigned long combo(unsigned long operand)
    {
        switch(operand)
        {
            case(0):
            case(1):
            case(2):
            case(3):
                return operand;
            case(4):
                return a;
            case(5):
                return b;
            case(6):
                return c;
            case(7):
                throw runtime_error("Reserved and will not appear in valid programs");
        }
        throw runtime_error("Invalid operand");
    }

    void execute(Opcode opcode, unsigned long operand)
    {
        auto comboOperand = combo(operand);
        if      (opcode == Opcode::Adv) a /= pow(2, comboOperand);
        else if (opcode == Opcode::Bxl) b ^= operand;
        else if (opcode == Opcode::Bst) b = comboOperand % 8;
        else if (opcode == Opcode::Jnz) {
            if(a)
            {
                p = operand;
                return;
            }
        }
        else if (opcode == Opcode::Bxc) b ^= c; // Ignore operand (legacy reasons)
        else if (opcode == Opcode::Out)
        {
            out.emplace_back(comboOperand % 8);
        }
        else if (opcode == Opcode::Bdv) b = a / pow(2, comboOperand);
        else if (opcode == Opcode::Cdv) c = a / pow(2, comboOperand);
        p += 2ul;
    }

    string run()
    {
        for(auto it = program.begin(); it != program.end();)
        {
            auto op = *it;
            auto operand = *(it+1);
            execute(static_cast<Opcode>(op), operand);
            it = program.begin() + p;
        }

        string outStr;
        for(const auto& o : out)
        {
            if(outStr.size() > 0)
            {
                outStr += ",";
            }
            outStr += to_string(o);
        }
        return outStr;
    }

    // FIXME: This works but makes multiple assumptions based on the program input...
    static bool solve(Computer computer, long p, unsigned long reg)
    {
        if(p < 0)
        {
            cout << reg << endl;
            return true;
        }
        for(auto d : views::iota(0, 8))
        {
            const auto& program = computer.program;
            computer.a = reg << 3 | d;
            computer.b = 0ul;
            computer.c = 0ul;
            computer.p = 0ul;
            while(computer.p < program.size())
            {
                const auto opcode = static_cast<Opcode>(program[computer.p]);
                const auto operand = program[computer.p+1];
                computer.execute(opcode, operand);
                if(static_cast<Opcode>(opcode) == Opcode::Out) break;
            }
            if(computer.out.back() == program[p] && solve(computer, p-1, reg << 3 | d))
            {
                return true;
            }
        }
        return false;
    }

    void solve()
    {
        solve(*this, program.size()-1, 0);
    }
};

Computer parse(istream& input)
{
    auto registerA = 0ul;
    auto registerB = 0ul;
    auto registerC = 0ul;
    vector<uint8_t> program;

    regex registerRegex(R"(Register\s([A-C]):\s(\d+))");
    regex programRegex(R"(Program:\s((\d+,?)+))");
    string line;
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
    getline(input, line); // Skip the \n
    getline(input, line);
    if(regex_match(line, match, programRegex))
    {
        string programStr = match[1].str();
        for (auto&& range : programStr | views::split(','))
        {
            string_view sv(&*range.begin(), ranges::distance(range));
            program.emplace_back(stoul(string(sv)));
        }
    }

    return {registerA, registerB, registerC, program};
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
    auto& input = file;

    auto computer = parse(input);
    cout << "Part 1: " << computer.run() << endl;
    cout << "Part 2: ";
    computer.solve(); // Will print part 2
}
