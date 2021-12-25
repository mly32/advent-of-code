#include <bits/stdc++.h>
using namespace std;

struct solution {
  vector<tuple<int, int, int, int>> lines;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      size_t c1 = line.find(',');
      size_t s1 = line.find(' ', c1 + 1);
      size_t s2 = line.find(' ', s1 + 1);
      size_t c2 = line.find(',', s2 + 1);
      int x1 = stoi(line.substr(0, c1));
      int y1 = stoi(line.substr(c1 + 1, s1 - c1 - 1));
      int x2 = stoi(line.substr(s2 + 1, c2 - s2 - 1));
      int y2 = stoi(line.substr(c2 + 1));
      lines.push_back({x1, y1, x2, y2});
    }

    input_file.close();
  }

  int solve1(const bool diag = false) {
    int mx_x = 0, mx_y = 0;
    for (const auto& [x1, y1, x2, y2] : lines) {
      mx_x = max({mx_x, x1, x2}), mx_y = max({mx_y, y1, y2});
    }
    vector<vector<int>> grid(mx_x + 1, vector<int>(mx_y + 1));

    for (const auto& [x1, y1, x2, y2] : lines) {
      if (x1 == x2) {
        int start = min(y1, y2), end = max(y1, y2) + 1;
        for (int j = start; j < end; ++j) {
          ++grid[x1][j];
        }
      } else if (y1 == y2) {
        int start = min(x1, x2), end = max(x1, x2) + 1;
        for (int i = start; i < end; ++i) {
          ++grid[i][y1];
        }
      } else if (diag) {
        if ((x1 <= x2 && y1 <= y2) || (x1 >= x2 && y1 >= y2)) {
          int start_x = min(x1, x2), end_x = max(x1, x2) + 1;
          for (int i = start_x, j = min(y1, y2); i < end_x; ++i, ++j) {
            ++grid[i][j];
          }
        } else {
          int start_x = min(x1, x2), end_x = max(x1, x2) + 1;
          for (int i = start_x, j = max(y1, y2); i < end_x; ++i, --j) {
            ++grid[i][j];
          }
        }
      }
    }
    int res = 0;
    for (int j = 0; j <= mx_y; ++j) {
      for (int i = 0; i <= mx_x; ++i) {
        // cout << (grid[i][j] != 0 ? to_string(grid[i][j]) : ".");
        res += grid[i][j] > 1;
      }
      // cout << endl;
    }
    printf("res: %d\n", res);
    return res;
  }

  int solve2() { return solve1(true); }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day05.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
