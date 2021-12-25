#include <bits/stdc++.h>
using namespace std;

struct solution {
  int x1, x2, y1, y2;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    getline(input_file, line);

    size_t x1_start = line.find_first_of("=") + 1;
    size_t x1_end = line.find_first_of(".", x1_start);
    x1 = stoi(line.substr(x1_start, x1_end - x1_start));

    size_t x2_start = x1_end + 2;
    size_t x2_end = line.find_first_of(",", x2_start);
    x2 = stoi(line.substr(x2_start, x2_end - x2_start));

    size_t y1_start = line.find_first_of("=", x2_end) + 1;
    size_t y1_end = line.find_first_of(".", y1_start);
    y1 = stoi(line.substr(y1_start, y1_end - y1_start));

    size_t y2_start = y1_end + 2;
    size_t y2_end = line.find_first_of(",", y2_start);
    y2 = stoi(line.substr(y2_start, y2_end - y2_start));

    input_file.close();
  }

  pair<bool, int> is_valid(int dx, int dy) {
    int x = 0, y = 0, max_y = 0;
    while (y >= y1) {
      max_y = max(max_y, y);
      if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
        return {true, max_y};
      }
      x += dx, y += dy;
      dx += dx > 0 ? -1 : dx < 0 ? 1 : 0, --dy;
    }

    return {false, max_y};
  }

  int solve() {
    int max_y = 0, valid_cnt = 0;
    printf("x1: %d, x2: %d, y1: %d, y2: %d\n", x1, x2, y1, y2);
    for (int dx = 0; dx < 1000; ++dx) {
      for (int dy = -1000; dy < 1000; ++dy) {
        auto [valid, mx_y] = is_valid(dx, dy);
        if (valid) {
          ++valid_cnt, max_y = max(max_y, mx_y);
        }
      }
    }
    printf("max y: %d, valid trajectories: %d\n", max_y, valid_cnt);
    return 0;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day17.txt";
  solution(filename).solve();
}
