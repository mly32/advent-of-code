#include <bits/stdc++.h>
using namespace std;

struct solution {
  enum loc_t { EAST = 0, SOUTH = 1, EMPTY = 2 };
  vector<vector<loc_t>> grid;
  array<vector<int>, 2> objs;
  array<vector<vector<int>>, 2> adjs;
  int m, n;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        grid.push_back({});
        for (const char c : line) {
          grid.back().push_back(c == '>' ? EAST : (c == 'v' ? SOUTH : EMPTY));
        }
      }
    }

    m = grid.size(), n = grid[0].size();
    adjs[0].resize(m * n), adjs[1].resize(m * n);

    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (grid[i][j] != EMPTY) {
          objs[grid[i][j]].push_back(n * i + j);
        }
        adjs[EAST][n * i + j].push_back(n * i + ((j + 1) % n));
        adjs[SOUTH][n * i + j].push_back(n * ((i + 1) % m) + j);
      }
    }

    input_file.close();
  }

  void print_grid() {
    for (const vector<loc_t>& row : grid) {
      for (const loc_t& c : row) {
        cout << (c == EAST ? '>' : (c == SOUTH ? 'v' : '.'));
      }
      cout << endl;
    }
    cout << endl;
  }

  inline loc_t& at(const int& p) { return grid[p / n][p % n]; }

  bool move() {
    bool same = true;
    for (int dir = 0; dir < 2; ++dir) {
      vector<pair<int, int>> moved;
      for (int& obj : objs[dir]) {
        for (const int& nei : adjs[dir][obj]) {
          if (at(nei) == EMPTY) {
            moved.push_back({obj, nei});
            obj = nei;
          }
        }
      }
      for (const auto& [obj, nei] : moved) {
        swap(at(obj), at(nei));
      }
      same &= moved.empty();
    }
    // print_grid();
    return same;
  }

  int solve1() {
    int i = 0;
    for (bool same = false; !same; ++i) {
      same = move();
    }
    int res = i;
    printf("res: %d\n", res);
    return res;
  }

  int solve2() {
    //
    return 0;
  }
};

int main() {
  const bool final = 1, snd_question = 0;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day25.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
