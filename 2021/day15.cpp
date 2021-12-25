#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

struct solution {
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

  vector<vector<pii>> get_adj() {
    int m = grid.size(), n = grid[0].size();
    vector<vector<pii>> adj;
    pii dirs[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    adj.resize(m * n);
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        for (auto [dx, dy] : dirs) {
          int x = i + dx, y = j + dy;
          if (x >= 0 && x < m && y >= 0 && y < n) {
            adj[n * i + j].push_back({n * x + y, grid[x][y]});
          }
        }
      }
    }
    return adj;
  }

  int solve1() {
    vector<vector<pii>> adj = get_adj();
    const int sz = adj.size();
    vector<int> dist(sz, INT_MAX);
    dist[0] = 0;

    priority_queue<pii, vector<pii>, greater<pii>> q;
    q.push({0, 0});
    while (!q.empty()) {
      auto [d_v, v] = q.top();
      q.pop();
      if (dist[v] != d_v) {
        continue;
      }

      for (auto [u, d] : adj[v]) {
        if (dist[v] + d < dist[u]) {
          dist[u] = dist[v] + d;
          q.push({dist[u], u});
        }
      }
    }

    printf("res: %d\n", dist[sz - 1]);
    return dist[sz - 1];
  }

  int solve1_dense() {
    vector<vector<pii>> adj = get_adj();
    const int sz = adj.size();
    vector<int> dist(sz, INT_MAX);
    vector<bool> seen(sz);
    dist[0] = 0;

    for (int i = 0; i < sz; ++i) {
      int v = -1;  // unseen vertex with min dist
      for (int j = 0; j < sz; ++j) {
        if (!seen[j] && (v == -1 || dist[j] < dist[v])) {
          v = j;
        }
      }

      if (dist[v] == INT_MAX) {
        break;
      }

      seen[v] = true;
      for (auto [u, d] : adj[v]) {
        if (dist[v] + d < dist[u]) {
          dist[u] = dist[v] + d;
        }
      }
    }

    printf("res: %d\n", dist[sz - 1]);
    return dist[sz - 1];
  }

  int solve2() {
    int m = grid.size(), n = grid[0].size();
    vector<vector<int>> nxt_grid(5 * m, vector<int>(5 * n));

    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        for (int x = 0; x < m; ++x) {
          for (int y = 0; y < n; ++y) {
            nxt_grid[i * m + x][j * n + y] = 1 + (grid[x][y] + i + j + 8) % 9;
          }
        }
      }
    }

    grid = nxt_grid;
    return solve1();
  }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day15.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}

// TODO learn dense dijkstra
