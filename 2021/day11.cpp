#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

struct solution {
  vector<vector<int>> grid;
  vector<int> energy;
  vector<vector<int>> neighbors;

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
          grid.back().push_back(c - '0');
        }
      }
    }

    int m = grid.size(), n = grid[0].size();
    energy.resize(m * n);
    neighbors.resize(m * n);
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        energy[n * i + j] = grid[i][j];
        const pii dirs[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                             {0, 1},   {1, -1}, {1, 0},  {1, 1}};
        for (auto [dx, dy] : dirs) {
          int x = i + dx, y = j + dy;
          if (x >= 0 && x < m && y >= 0 && y < n) {
            neighbors[n * i + j].push_back(n * x + y);
          }
        }
      }
    }
    // for (int i = 0; i < m * n; ++i) {
    //   cout << i << ": ";
    //   for (auto x : neighbors[i]) {
    //     cout << (x / m) << "," << (x % m) << "  ";
    //   }
    //   cout << endl;
    // }

    input_file.close();
  }

  void print() {
    const int sz = energy.size();
    int m = grid.size();
    for (int i = 0; i < sz; ++i) {
      cout << energy[i] << " ";
      if (i % m == m - 1) {
        cout << endl;
      }
    }
  }

  int solve() {
    const int sz = energy.size();
    int res = 0, ans = 0;

    for (int step = 0; ans == 0; ++step) {
      int flashes = 0;
      vector<bool> flashed(sz);
      for (int i = 0; i < sz; ++i) {
        ++energy[i];
      }
      queue<int> q;
      for (int i = 0; i < sz; ++i) {
        if (energy[i] > 9 && !flashed[i]) {
          q.push(i);
          flashed[i] = true;
          while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const int& v : neighbors[u]) {
              ++energy[v];
              if (energy[v] > 9 && !flashed[v]) {
                flashed[v] = true;
                q.push(v);
              }
            }
          }
        }
      }
      for (int i = 0; i < sz; ++i) {
        if (flashed[i]) {
          ++flashes;
          energy[i] = 0;
        }
      }
      if (flashes == sz) {
        ans = step + 1;
      }
      if (step < 100) {
        res += flashes;
      }
    }
    printf("res: %d, ans: %d\n", res, ans);
    return res;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day11.txt";
  solution(filename).solve();
}

// TODO learn dense dijkstra
