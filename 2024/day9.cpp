#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct Chunk
{
    unsigned int index; // to blockFormat memory
    size_t size;
};

void printBlockFormat(const vector<unsigned int>& blockFormat)
{
    for(const auto& b : blockFormat)
    {
        // b is using one-based indexing, with 0 representing a free block
        const string id = !b ? "." : to_string(b-1);
        cout << id;
    }
    cout << endl;
}

auto calculateChecksum(const vector<unsigned int>& blockFormat)
{
    auto checksum = 0ul;
    for(auto i = 0u; i < blockFormat.size(); ++i)
    {
        // blockFormat[i] is using one-based indexing, with 0 representing a free block
        const auto id = !blockFormat[i] ? 0 : blockFormat[i]-1;
        checksum += i * id;
    }
    return checksum;
}

auto part1(vector<unsigned int> blockFormat)
{
    auto i = 0u;
    auto j = blockFormat.size()-1;
    while(true)
    {
        while(blockFormat[i]) // file block
        {
            i++;
        }
        while(!blockFormat[j]) // free block
        {
            j--;
        }
        if(i > j) // processed blocks
        {
            break;
        }
        swap(blockFormat[i], blockFormat[j]);
    }
    return calculateChecksum(blockFormat);
}

auto part2(vector<unsigned int> blockFormat, list<Chunk>& freeChunks, stack<Chunk>& fileChunks)
{
    while(!fileChunks.empty())
    {
        const auto fileChunk = fileChunks.top();
        fileChunks.pop();
        for (auto freeIt = freeChunks.begin(); freeIt != freeChunks.end() && freeIt->index < fileChunk.index;)
        {
            const int freeSpace = freeIt->size - fileChunk.size;
            if(freeSpace >= 0)
            {
                const auto freeBlockBegin = blockFormat.begin()+freeIt->index;
                const auto freeBlockEnd = freeBlockBegin+fileChunk.size; // using fileChunk.size as our fill size
                const auto fileBlockBegin = blockFormat.begin()+fileChunk.index;
                const auto fileBlockEnd = fileBlockBegin+fileChunk.size;
                const auto id = blockFormat[fileChunk.index];
                fill(freeBlockBegin, freeBlockEnd, id);
                fill(fileBlockBegin, fileBlockEnd, 0);
                freeIt->index += fileChunk.size;
                freeIt->size = freeSpace;
                if(freeSpace == 0) // cleaning out 0 free space nodes from our list
                {
                    freeChunks.erase(freeIt);
                }
                break;
            }
            freeIt++;
        }
    }
    return calculateChecksum(blockFormat);
}

int main(int argc, char** argv)
{
    stringstream example(R"(2333133121414131402)");
    ifstream file("../day9.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string denseFormat;
    if(!getline(input, denseFormat))
    {
        cerr << "Error reading file" << "\n";
    }

    // Using one-based indexing for idNumber with zero representing free space
    auto idNumber = 1u;
    vector<unsigned int> blockFormat;
    list<Chunk> freeChunks;
    stack<Chunk> fileChunks;
    // TODO: Possibly could have used a single list<Chunk> for both free and file chunks
    for(auto i = 0u; i < denseFormat.size(); ++i)
    {
        const auto fileBlock = !(i % 2);
        auto size = denseFormat[i] - '0';
        auto id = 0u;
        auto index = blockFormat.size();
        if(fileBlock && size)
        {
            id = idNumber++;
            fileChunks.emplace(index, size);
        }
        else if(!fileBlock && size)
        {
            // id = 0u;
            freeChunks.emplace_back(index, size);
        }
        while(size--)
        {
            blockFormat.emplace_back(id);
        }
    }
    auto checksumP1 = part1(blockFormat);
    auto checksumP2 = part2(blockFormat, freeChunks, fileChunks);
    cout << checksumP1 << endl;
    cout << checksumP2 << endl;
}
