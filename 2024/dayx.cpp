#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
    stringstream example(
R"()");
    ifstream file("../dayx.txt");
    if (!file)
    {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = example;
    string line;
    while (getline(input, line))
    {
    }
}
