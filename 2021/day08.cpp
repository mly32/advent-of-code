#include <bits/stdc++.h>
using namespace std;

struct solution {
  vector<vector<string>> input;

  solution(const string& filename) {
    std::ifstream fin(filename);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string str = buffer.str();
    transform(str.begin(), str.end(), str.begin(), [](const char c) {
      if (c == '\n') {
        return '|';
      }
      return c;
    });

    input.push_back({});
    for (size_t i = 0, j = str.find_first_of(" |"); j != std::string::npos;
         i = j + 1, j = str.find_first_of(" |", i)) {
      if (i < j) {
        input.back().emplace_back(str.substr(i, j - i));
      }
      if (str[j] == '|' && !input.back().empty()) {
        input.push_back({});
      }
    }

    fin.close();
  }

  inline set<char> z(const string& s) { return set<char>(s.begin(), s.end()); }

  const map<int, vector<pair<int, set<char>>>> mp = {
      {2, {{1, z("cf")}}},
      {3, {{7, z("acf")}}},
      {4, {{4, z("bcdf")}}},
      {5, {{2, z("acdeg")}, {3, z("acdfg")}, {5, z("abdfg")}}},
      {6, {{0, z("abcefg")}, {6, z("abdefg")}, {9, z("abcdfg")}}},
      {7, {{8, z("abcdefg")}}},
  };

  const map<set<char>, int> to_val = {
      {z("abcefg"), 0},  {z("cf"), 1},     {z("acdeg"), 2},  {z("acdfg"), 3},
      {z("bcdf"), 4},    {z("abdfg"), 5},  {z("abdefg"), 6}, {z("acf"), 7},
      {z("abcdefg"), 8}, {z("abcdfg"), 9},
  };

  const vector<pair<int, char>> info = {{1, 'c'}, {7, 'a'}, {4, 'd'}, {8, 'g'}};

  int solve() {
    const int sz = input.size();
    int res = 0, res2 = 0;

    for (int i = 0; i < sz - 1; i += 2) {
      vector<string>& key = input[i];
      vector<string>& msg = input[i + 1];

      map<char, int> cnts;
      map<int, set<char>> num_to_chars;

      for (const string& k : key) {
        for (auto c : k) {
          ++cnts[c];
        }
        if (mp.at(k.size()).size() == 1) {
          num_to_chars[mp.at(k.size())[0].first] = z(k);
        }
      }

      map<char, char> segment_map;

      for (auto [k, cnt] : cnts) {
        // cout << k << ": " << cnt << endl;
        if (cnt == 6) {
          segment_map[k] = 'b';
        } else if (cnt == 4) {
          segment_map[k] = 'e';
        } else if (cnt == 9) {
          segment_map[k] = 'f';
        }
      }

      for (const auto& [num, val] : info) {
        for (const char c : num_to_chars[num]) {
          if (segment_map.find(c) == segment_map.end()) {
            segment_map[c] = val;
          }
        }
      }
      int n = 0;
      for (string& m : msg) {
        transform(m.begin(), m.end(), m.begin(),
                  [&segment_map](const char c) { return segment_map.at(c); });
        n = n * 10 + to_val.at(z(m));

        res += mp.at(m.size()).size() == 1;
      }
      res2 += n;
    }

    printf("res: %d, %d\n", res, res2);
    return res;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day08.txt";
  solution(filename).solve();
}
