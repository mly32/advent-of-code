#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  vector<vector<char>> input;
  vector<tuple<int, int, int>> moves;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    bool read_moves = false;

    while (getline(input_file, line)) {
      if (line != "") {
        if (!read_moves) {
          size_t sz = (line.size() + 3) / 4;
          input.resize(max(input.size(), sz));
          for (size_t i = 0; i < sz; ++i) {
            if (line[i * 4 + 1] != ' ') {
              input[i].push_back(line[i * 4 + 1]);
            }
          }
        } else {
          vector<int> pos = {-1};
          for (int i = 0; i < 5; ++i) {  // 5 spaces
            pos.push_back(line.find(' ', pos.back() + 1));
          }
          pos.push_back(line.size());

          vector<string> strs;
          for (size_t i = 0; i < pos.size() - 1; ++i) {
            strs.push_back(line.substr(pos[i] + 1, pos[i + 1] - pos[i]));
          }
          moves.push_back(
              {stoi(strs[1]), stoi(strs[3]) - 1, stoi(strs[5]) - 1});
        }
      } else {
        read_moves = true;
      }
    }

    for (auto& v : input) {
      v.pop_back();
      reverse(v.begin(), v.end());
    }
    input_file.close();
  }

  void solve1() {
    string res;

    for (auto [cnt, from, to] : moves) {
      for (int i = 0; i < cnt; ++i) {
        input[to].push_back(input[from].back());
        input[from].pop_back();
      }
    }

    for (auto& v : input) {
      res.push_back(v.back());
    }

    printf("res: %s\n", res.c_str());
  }

  void solve2() {
    string res;

    for (auto [cnt, from, to] : moves) {
      for (int i = 0; i < cnt; ++i) {
        input[to].push_back(input[from][input[from].size() - cnt + i]);
      }
      for (int i = 0; i < cnt; ++i) {
        input[from].pop_back();
      }
    }

    for (auto& v : input) {
      res.push_back(v.back());
    }

    printf("res: %s\n", res.c_str());
  }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "../input/" + string(final ? "final" : "practice") + "/day" + day + ".in";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
