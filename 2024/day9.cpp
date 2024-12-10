#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>

using namespace std;

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

    auto idNumber = 1u; // Starting at 1 for file block id
    vector<unsigned int> blockFormat;
    struct Chunk
    {
        unsigned int index;
        size_t size;
    };
    list<Chunk> freeChunks;
    stack<Chunk> fileChunks;
    for(auto i = 0u; i < denseFormat.size(); ++i)
    {
        const auto fileBlock = !(i % 2);
        auto size = denseFormat[i] - '0';
        auto id = 0u; // Representing 0 as a free block
        auto index = blockFormat.size();
        if(fileBlock && size)
        {
            id = idNumber++;
            fileChunks.emplace(index, size);
        }
        else if(!fileBlock && size)
        {
            id = 0u;
            freeChunks.emplace_back(index, size);
        }
        while(size--)
        {
            blockFormat.emplace_back(id);
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
//    auto i = 0u;
//    auto j = blockFormat.size()-1;
//    while(true)
//    {
//        while(blockFormat[i])
//        {
//            i++;
//        }
//        while(!blockFormat[j])
//        {
//            j--;
//        }
//        if(i > j)
//        {
//            break;
//        }
//        swap(blockFormat[i], blockFormat[j]);
//        //cout << i << ", " << j << endl;
//        //printBlocks();
//    }
    printBlocks();
    while(!fileChunks.empty())
    {
        const auto fileChunk = fileChunks.top();
        fileChunks.pop();
        for (auto it = freeChunks.begin(); it != freeChunks.end() && it->index < fileChunk.index;)
        {
            const auto freeChunk = *it;
            const int freeSpace = freeChunk.size - fileChunk.size;
            if(freeSpace >= 0)
            {
                const auto freeBlockBegin = blockFormat.begin()+freeChunk.index;
                const auto freeBlockEnd = freeBlockBegin+fileChunk.size; // fileBlock is always smaller
                const auto fileBlockBegin = blockFormat.begin()+fileChunk.index;
                const auto fileBlockEnd = fileBlockBegin+fileChunk.size;
                const auto id = blockFormat[fileChunk.index];
                fill(freeBlockBegin, freeBlockEnd, id);
                fill(fileBlockBegin, fileBlockEnd, 0);
                it->index += fileChunk.size;
                it->size = freeSpace;
                if(freeSpace == 0)
                {
                    freeChunks.erase(it);
                }
                break;
            }
            it++;
        }
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
