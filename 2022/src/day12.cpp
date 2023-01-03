#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  using pq_t = tuple<int, int, int>;
  vector<int> dirs = {1, 0, -1, 0, 1};
  vector<vector<int>> grid;
  pair<int, int> src;
  pair<int, int> tgt;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    int i = 0;
    while (getline(input_file, line)) {
      if (line != "") {
        grid.push_back(vector<int>(line.size()));
        for (int j = 0; j < (int)line.size(); ++j) {
          int height = line[j] - 'a';
          if (line[j] == 'S') {
            height = 0;
            src = {i, j};
          } else if (line[j] == 'E') {
            height = 25;
            tgt = {i, j};
          }
          grid.back()[j] = height;
        }
        i++;
      }
    }

    input_file.close();
  }

  void solve1(const bool ver1 = true) {
    ll res = 0;
    int m = grid.size(), n = grid[0].size();

    vector<vector<int>> dist(m, vector<int>(n, 1e9));
    priority_queue<pq_t, vector<pq_t>, greater<pq_t>> pq;

    auto [x, y] = ver1 ? src : tgt;
    dist[x][y] = 0;
    pq.push({0, x, y});

    // dijkstra's shortest path
    // (once node is used, distance is shortest and will not change)
    while (!pq.empty()) {
      auto [d, i, j] = pq.top();
      pq.pop();
      for (int k = 0; k < 4; ++k) {
        ll r = i + dirs[k], c = j + dirs[k + 1];
        if (r >= 0 && r < m && c >= 0 && c < n) {
          int cost = ver1 ? grid[r][c] - grid[i][j] : grid[i][j] - grid[r][c];
          if (cost <= 1 && dist[r][c] > dist[i][j] + 1) {
            dist[r][c] = dist[i][j] + 1;
            pq.push({dist[r][c], r, c});
          }
        }
      }
    }

    if (ver1) {
      res = dist[get<0>(tgt)][get<1>(tgt)];
    } else {
      res = 1e9;
      for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
          if (grid[i][j] == 0 && dist[i][j] < res) {
            res = dist[i][j];
          }
        }
      }
    }

    printf("res: %lld\n", res);
  }

  void solve2() { solve1(false); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
