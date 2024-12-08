#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

auto createAntinodeMap(const vector<string>& lines, bool includeHarmonics = false)
{
    unordered_map<char, vector<pair<int,int>>> antennaMap;
    const int rows = lines.size();
    const int cols = lines[0].size();
    for(auto rrr = 0u; rrr < rows; ++rrr)
    {
        for(auto ccc = 0u; ccc < cols; ++ccc)
        {
            const auto c = lines[rrr][ccc];
            if(c != '.') // is it antenna?
            {
                auto it = antennaMap.find(c);
                if(it != antennaMap.end())
                {
                    it->second.emplace_back(rrr, ccc);
                }
                else // antenna doesn't exist in map yet...
                {
                    antennaMap.emplace(c, vector<pair<int,int>>{{rrr, ccc}});
                }
            }
        }
    }

    auto inBounds = [&rows, &cols](pair<int,int> p){
        return p.first >= 0 && p.second >= 0 && p.first < rows && p.second < cols;
    };
    unordered_map<char, vector<pair<int,int>>> antinodeMap;
    for(const auto& [a, positions] : antennaMap)
    {
        antinodeMap.emplace(a, vector<pair<int,int>>{});
        const auto it = antinodeMap.find(a);
        for(auto i = 0; i < positions.size(); ++i)
        {
            for(auto j = i + 1; j < positions.size(); ++j)
            {
                const auto deltaRRR = positions[i].first - positions[j].first;
                const auto deltaCCC = positions[i].second - positions[j].second;

                if(includeHarmonics)
                {
                    it->second.emplace_back(positions[i]);
                    it->second.emplace_back(positions[j]);  // TODO: May be double counting
                    pair<int,int> antinodeI{positions[i].first + deltaRRR, positions[i].second + deltaCCC};
                    while(inBounds(antinodeI))
                    {
                        it->second.emplace_back(antinodeI);
                        antinodeI = {antinodeI.first + deltaRRR, antinodeI.second + deltaCCC};
                    }
                    pair<int,int> antinodeJ{positions[j].first - deltaRRR, positions[j].second - deltaCCC};
                    while(inBounds(antinodeJ))
                    {
                        it->second.emplace_back(antinodeJ);
                        antinodeJ = {antinodeJ.first - deltaRRR, antinodeJ.second - deltaCCC};
                    }
                }
                else
                {
                    pair<int,int> antinodeI{positions[i].first + deltaRRR, positions[i].second + deltaCCC};
                    if(inBounds(antinodeI))
                    {
                        it->second.emplace_back(antinodeI);
                    }
                    pair<int,int> antinodeJ{positions[j].first - deltaRRR, positions[j].second - deltaCCC};
                    if(inBounds(antinodeJ))
                    {
                        it->second.emplace_back(antinodeJ);
                    }
                }
            }
        }
    }
    return antinodeMap;
}

auto uniqueAntinodePositions(const unordered_map<char, vector<pair<int,int>>>& antinodeMap)
{
    set<pair<int,int>> antinodeSet;
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
