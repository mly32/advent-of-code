#include <bits/stdc++.h>
using namespace std;

struct solution {
  enum DIR : char { X = 'X', Y = 'Y' };
  set<pair<int, int>> points;
  vector<pair<DIR, int>> folds;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      size_t comma = line.find_first_of(',');
      if (comma != string::npos) {
        points.emplace(stoi(line.substr(0, comma)),
                       stoi(line.substr(comma + 1)));

      } else {
        size_t equal = line.find_first_of('=');
        if (equal != string::npos) {
          folds.emplace_back(line[equal - 1] == 'x' ? X : Y,
                             stoi(line.substr(equal + 1)));
        }
      }
    }

    input_file.close();
  }

  size_t solve() {
    size_t res = points.size();
    for (const auto& [dir, i] : folds) {
      set<pair<int, int>> nxt_points;
      for (const auto& [x, y] : points) {
        if (dir == X && x > i) {
          nxt_points.emplace(2 * i - x, y);
        } else if (dir == Y && y > i) {
          nxt_points.emplace(x, 2 * i - y);
        } else if ((dir == X && x != i) || (dir == Y && y != i)) {
          nxt_points.emplace(x, y);
        }
      }

      points = nxt_points;
      res = points.size();
      printf("res: %lu\n", res);
    }
    int max_x = 0, max_y = 0;
    for (const auto& [x, y] : points) {
      max_x = max(max_x, x);
      max_y = max(max_y, y);
    }

    vector<vector<bool>> grid(++max_y, vector<bool>(++max_x));

    for (const auto& [x, y] : points) {
      grid[y][x] = true;
    }

    for (int i = 0; i < max_y; ++i) {
      for (int j = 0; j < max_x; ++j) {
        cout << (grid[i][j] ? '#' : ' ');
      }
      cout << endl;
    }

    printf("res: %lu\n", res);
    return res;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day13.txt";
  solution(filename).solve();
}
