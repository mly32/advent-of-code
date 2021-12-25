#include <bits/stdc++.h>
using namespace std;

struct solution {
  vector<int> nums;
  vector<vector<int>> boards;
  const int lane_sz = 5, board_sz = lane_sz * lane_sz;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    getline(input_file, line);
    line.push_back(',');
    for (size_t i = 0, j = line.find(','); j != std::string::npos;
         i = j + 1, j = line.find(',', i)) {
      if (i < j) {
        nums.emplace_back(stoi(line.substr(i, j - i)));
      }
    }
    int line_cnt = 0;
    while (getline(input_file, line)) {
      if (line != "") {
        if (line_cnt % 5 == 0) {
          boards.push_back({});
        }
        line.push_back(' ');
        for (size_t i = 0, j = line.find(' '); j != std::string::npos;
             i = j + 1, j = line.find(' ', i)) {
          if (i < j) {
            boards.back().emplace_back(stoi(line.substr(i, j - i)));
          }
        }
        ++line_cnt;
      }
    }

    input_file.close();
  }

  int solve() {
    vector<vector<int>> pos_to_lanes(board_sz);
    for (int i = 0; i < lane_sz; ++i) {
      for (int j = 0; j < lane_sz; ++j) {
        pos_to_lanes[lane_sz * i + j] = {lane_sz + i, j};
      }
    }

    int sz = boards.size();
    vector<vector<set<int>>> board_lanes(sz, vector<set<int>>(2 * lane_sz));

    vector<map<int, set<int>>> num_to_pos(sz);
    vector<vector<int>> marked(sz, vector<int>(board_sz));
    vector<bool> complete(sz);
    for (int i = 0; i < sz; ++i) {
      for (int j = 0; j < board_sz; ++j) {
        num_to_pos[i][boards[i][j]].insert(j);
      }
    }

    vector<int> scores;

    for (const int n : nums) {
      for (int i = 0; i < sz; ++i) {
        if (!complete[i] && !num_to_pos[i][n].empty()) {
          for (const int pos : num_to_pos[i][n]) {
            marked[i][pos] = true;

            for (const int lane : pos_to_lanes[pos]) {
              board_lanes[i][lane].insert(n);
              if ((int)board_lanes[i][lane].size() == lane_sz) {
                int unmarked = 0;
                for (int j = 0; j < board_sz; ++j) {
                  unmarked += marked[i][j] ? 0 : boards[i][j];
                }
                scores.push_back(n * unmarked);
                complete[i] = true;
              }
            }
          }
        }
      }
    }

    printf("first: %d, last: %d\n", *scores.begin(), *scores.rbegin());

    return 0;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day04.txt";
  solution(filename).solve();
}
