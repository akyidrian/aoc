#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

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

    unordered_multimap<char, vector<pair<int,int>>> antennaMap;
    const int rows = lines.size();
    const int cols = lines[0].size();
    for(auto rrr = 0u; rrr < rows; ++rrr)
    {
        for(auto ccc = 0u; ccc < cols; ++ccc)
        {
            const auto c = lines[rrr][ccc];
            if(c != '.')
            {
                auto it = antennaMap.find(c);
                if(it != antennaMap.end())
                {
                    it->second.emplace_back(rrr, ccc);
                }
                else
                {
                    antennaMap.emplace(c, vector<pair<int,int>>{{rrr, ccc}});
                }
            }
        }
    }

    auto inBounds = [&rows, &cols](pair<int,int> p){
        return p.first >= 0 && p.second >= 0 && p.first < rows && p.second < cols;
    };
    unordered_multimap<char, vector<pair<int,int>>> antinodeMap;
    for(const auto& [a, positions] : antennaMap)
    {
        antinodeMap.emplace(a, vector<pair<int,int>>{});
        for(auto i = 0; i < positions.size(); ++i)
        {
            for(auto j = i + 1; j < positions.size(); ++j)
            {
                const auto deltaRRR = positions[i].first - positions[j].first;
                const auto deltaCCC = positions[i].second - positions[j].second;
                pair<int,int> antinodeI{positions[i].first + deltaRRR, positions[i].second + deltaCCC};
                pair<int,int> antinodeJ{positions[j].first - deltaRRR, positions[j].second - deltaCCC};

                auto it = antinodeMap.find(a);
                if(inBounds(antinodeI))
                {
                    it->second.emplace_back(antinodeI);
                }
                if(inBounds(antinodeJ))
                {
                    it->second.emplace_back(antinodeJ);
                }
            }
        }
// TODO:
//        for(const auto& p : positions)
//        {
//            cout << a << ": " << p.first << ", " << p.second << endl;
//        }
    }
    set<pair<int,int>> antinodeSet;
    for(const auto& [a, antinodePositions] : antinodeMap)
    {
        for(const auto& p : antinodePositions)
        {
            antinodeSet.insert(p);
        }
    }
    cout << antinodeSet.size() << endl;
}
