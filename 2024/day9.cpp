#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    stringstream example(
    R"(2333133121414131402)");
    ifstream file("../day9.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string denseFormat;
    if(!getline(input, denseFormat)) {
        cerr << "Error reading file" << "\n";
    }

    auto idNumber = 1u; // Starting at 1 for file block id
    vector<unsigned int> blockFormat;
    for(auto i = 0u; i < denseFormat.size(); ++i)
    {
        const auto fileBlock = !(i % 2);
        auto blocks = denseFormat[i] - '0';
        auto blockChar = 0u; // Representing 0 as a free block
        if(fileBlock)
        {
            blockChar = idNumber++;
        }
        while(blocks--)
        {
            blockFormat.push_back(blockChar);
        }
    }

    cout << denseFormat << endl;
    auto printBlocks = [&blockFormat](){
        for(auto b : blockFormat)
        {
            string id = !b ? "." : to_string(b-1);
            cout << id;
        }
        cout << endl;
    };
    auto i = 0u;
    auto j = blockFormat.size()-1;
    while(true)
    {
        while(blockFormat[i])
        {
            i++;
        }
        while(!blockFormat[j])
        {
            j--;
        }
        if(i > j)
        {
            break;
        }
        swap(blockFormat[i], blockFormat[j]);
        //cout << i << ", " << j << endl;
        //printBlocks();
    }

    unsigned long checksum = 0u;
    for(auto i = 0u; i < blockFormat.size(); ++i)
    {
        auto id = !blockFormat[i] ? 0 : blockFormat[i]-1;
        checksum += i * id;
    }
    printBlocks();
    cout << checksum << endl;
}
