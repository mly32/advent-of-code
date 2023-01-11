#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  enum orientation { row, col };
  enum dir { pos, neg };

  vector<string> input;

  vector<tuple<orientation, dir>> info = {
      {row, pos},
      {row, neg},
      {col, pos},
      {col, neg},
  };

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        input.push_back(line);
      }
    }

    input_file.close();
  }

  void solve1() {
    int res = 0;
    int m = input.size(), n = input[0].size();
    vector<vector<bool>> vis(m, vector<bool>(n));

    for (auto [o, d] : info) {
      int outer_size = o == row ? m : n, inner_size = o == row ? n : m;
      for (int i = 0; i < outer_size; ++i) {
        int max_height = -1;
        for (int j = d == pos ? 0 : inner_size - 1;
             d == pos ? j < inner_size : j >= 0; j += d == pos ? 1 : -1) {
          int r = o == row ? i : j, c = o == row ? j : i;
          if (input[r][c] > max_height) {
            max_height = input[r][c];
            vis[r][c] = true;
          }
        }
      }
    }

    for (auto& v : vis) {
      for (auto e : v) {
        res += e;
      }
    }

    printf("res: %d\n", res);
  }

  void solve2() {
    ll res = 0;

    ll m = input.size(), n = input[0].size();

    vector<ll> dirs = {1, 0, -1, 0, 1};
    for (ll i = 0; i < m; ++i) {
      for (ll j = 0; j < n; ++j) {
        vector<ll> lengths(4);
        for (ll k = 0; k < 4; ++k) {
          ll r = i, c = j;

          ll length = 0;
          r += dirs[k], c += dirs[k + 1];
          while (r >= 0 && r < m && c >= 0 && c < n) {
            length++;
            if (input[r][c] >= input[i][j]) {
              break;
            }
            r += dirs[k], c += dirs[k + 1];
          }
          lengths[k] = length;
        }
        res = max(res, lengths[0] * lengths[1] * lengths[2] * lengths[3]);
      }
    }

    printf("res: %lld\n", res);
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
