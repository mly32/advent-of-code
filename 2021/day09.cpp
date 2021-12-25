#include <bits/stdc++.h>
using namespace std;

struct solution {
  vector<int> values;
  vector<vector<int>> grid;

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

    input_file.close();
  }

  vector<vector<int>> get_adj() {
    int m = grid.size(), n = grid[0].size();
    vector<vector<int>> adj;
    pair<int, int> dirs[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    values.resize(m * n);
    adj.resize(m * n);
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        values[n * i + j] = grid[i][j];
        for (auto [dx, dy] : dirs) {
          int x = i + dx, y = j + dy;
          if (x >= 0 && x < m && y >= 0 && y < n) {
            adj[n * i + j].push_back(n * x + y);
          }
        }
      }
    }
    cout << "size: " << m << " " << n << endl;
    return adj;
  }

  int solve() {
    vector<vector<int>> adj = get_adj();
    const int sz = adj.size();
    vector<int> low_points;
    int low_score = 0;

    for (int u = 0; u < sz; ++u) {
      bool is_lower = true;
      for (const int v : adj[u]) {
        is_lower &= values[u] < values[v];
      }
      if (is_lower) {
        low_points.push_back(u);
        low_score += values[u] + 1;
      }
    }

    vector<int> areas;

    for (const int p : low_points) {
      vector<bool> seen(sz);
      queue<int> q;

      int area = 1;
      q.push(p);
      seen[p] = true;

      while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const int v : adj[u]) {
          if (!seen[v] && values[v] != 9) {
            seen[v] = true;
            ++area;
            q.push(v);
          }
        }
      }
      areas.push_back(area);
    }
    sort(areas.begin(), areas.end(), greater<int>());
    unsigned long long area = 1ull * areas[0] * areas[1] * areas[2];

    printf("res: %d, %llu\n", low_score, area);
    return low_score;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day09.txt";
  solution(filename).solve();
}

// TODO learn dense dijkstra
