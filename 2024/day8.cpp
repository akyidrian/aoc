#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

using Position = pair<int,int>; // row, col
using AntennaMap = unordered_map<char, vector<Position>>;
using AntinodeMap = AntennaMap;

auto createAntinodeMap(const vector<string>& lines, bool includeHarmonics = false)
{
    // Creating a map data structure that stores the positions of each kind of antenna
    AntennaMap antennaMap;
    for(auto rrr = 0u; rrr < lines.size(); ++rrr)
    {
        for(auto ccc = 0u; ccc < lines[0].size(); ++ccc)
        {
            const auto c = lines[rrr][ccc];
            if(c != '.') // is it antenna?
            {
                const auto it = antennaMap.find(c);
                if(it != antennaMap.end())
                {
                    it->second.emplace_back(rrr, ccc);
                }
                else // antenna doesn't exist in map yet...
                {
                    antennaMap.emplace(c, vector<Position>{{rrr, ccc}});
                }
            }
        }
    }

    // Creating a map data structure that stores the positions of each antinode generated
    // for each kind of antenna. Note, duplicate antinode positions between different kinds
    // of antenna may exist.
    const int rows = lines.size();
    const int cols = lines[0].size();
    auto inBounds = [&rows, &cols](Position p){
        return p.first >= 0 && p.second >= 0 && p.first < rows && p.second < cols;
    };
    AntinodeMap antinodeMap;
    for(const auto& [a, positions] : antennaMap)
    {
        antinodeMap.emplace(a, vector<Position>{});
        for(auto i = 0; i < positions.size(); ++i)
        {
            for(auto j = i + 1; j < positions.size(); ++j)
            {
                const auto deltaRRR = positions[i].first - positions[j].first;
                const auto deltaCCC = positions[i].second - positions[j].second;

                Position antinodeI{positions[i].first + deltaRRR, positions[i].second + deltaCCC};
                if(inBounds(antinodeI))
                {
                    antinodeMap[a].emplace_back(antinodeI);
                }
                Position antinodeJ{positions[j].first - deltaRRR, positions[j].second - deltaCCC};
                if(inBounds(antinodeJ))
                {
                    antinodeMap[a].emplace_back(antinodeJ);
                }
                if(includeHarmonics)
                {
                    // TODO: This will add duplicates of antenna positions into the map
                    //       which currently the uniqueAntinodePositions helps overcome
                    antinodeMap[a].emplace_back(positions[i]);
                    antinodeMap[a].emplace_back(positions[j]);
                    antinodeI = {antinodeI.first + deltaRRR, antinodeI.second + deltaCCC};
                    while(inBounds(antinodeI))
                    {
                        antinodeMap[a].emplace_back(antinodeI);
                        antinodeI = {antinodeI.first + deltaRRR, antinodeI.second + deltaCCC};
                    }
                    antinodeJ = {antinodeJ.first - deltaRRR, antinodeJ.second - deltaCCC};
                    while(inBounds(antinodeJ))
                    {
                        antinodeMap[a].emplace_back(antinodeJ);
                        antinodeJ = {antinodeJ.first - deltaRRR, antinodeJ.second - deltaCCC};
                    }
                }
            }
        }
    }
    return antinodeMap;
}

auto uniqueAntinodePositions(const AntinodeMap& antinodeMap)
{
    set<Position> antinodeSet;
    for (const auto& [_, antinodePositions] : antinodeMap) {
        antinodeSet.insert(antinodePositions.begin(), antinodePositions.end());
    }
    return antinodeSet;
}

int main(int argc, char** argv)
{
    stringstream example(
    R"(............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............)");
    ifstream file("../day8.txt");
    if (!file) {
        cerr << "Failed to open file" << "\n";
        return 1;
    }
    auto& input = file;
    string line;
    vector<string> lines; // aka city antenna map
    while (getline(input, line)) {
        lines.emplace_back(line);
    }

    const auto antinodePositionsP1 = uniqueAntinodePositions(createAntinodeMap(lines));
    const auto antinodePositionsP2 = uniqueAntinodePositions(createAntinodeMap(lines, true));
    cout << "Part 1: " << antinodePositionsP1.size() << endl;
    cout << "Part 2: " << antinodePositionsP2.size() << endl;
}
