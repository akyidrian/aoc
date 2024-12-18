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
    stringstream out;

    Computer() = delete;
    Computer(unsigned long a, unsigned long b, unsigned long c, const vector<uint8_t>& program)
    : a(a), b(b), c(c), program(program) {}

    Computer(const Computer& other)
        : a(other.a),
          b(other.b),
          c(other.c),
          program(other.program),
          p(other.p)
    {
        out.str(other.out.str());
        out.clear();
        out.setstate(other.out.rdstate());
    }

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
                if(out.str().size() > 0)
                {
                    out << ',';
                }
                out << comboOperand % 8;
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
        return out.str();
    }

    // FIXME: This works but makes multiple assumptions based on the program input...
    bool solve(const vector<uint8_t>& program, long p, unsigned long reg)
    {
        if(p < 0)
        {
            cout << reg << endl;
            return true;
        }
        for(auto d : views::iota(0, 8))
        {
            auto a = reg << 3 | d;
            auto b = 0ul;
            auto c = 0ul;
            auto out = 0ul;
            auto i = 0l;
            while(i < program.size())
            {
                auto o = 0ul;
                if      (program[i+1] <= 3) o = program[i+1];
                else if (program[i+1] == 4) o = a;
                else if (program[i+1] == 5) o = b;
                else if (program[i+1] == 6) o = c;

                if      (program[i] == 0) a >>= o;
                else if (program[i] == 1) b ^= program[i+1];
                else if (program[i] == 2) b = o & 7;
                else if (program[i] == 3)
                {
                    // TODO: We don't ever hit this case here...
                    //if(a != 0)
                    //{
                    //    i = program[i+1] - 2;
                    //}
                }
                else if (program[i] == 4) b ^= c;
                else if (program[i] == 5)
                {
                    out = o & 7;
                    break;
                }
                else if (program[i] == 6) b = a >> o;
                else if (program[i] == 7) c = a >> o;
                i += 2;
            }
            if(out == program[p] && solve(program, p-1, reg << 3 | d))
            {
                return true;
            }
        }
        return false;
    }

    void solve()
    {
        solve(program, program.size()-1, 0);
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
    computer.solve(); // Will print part 2
}
