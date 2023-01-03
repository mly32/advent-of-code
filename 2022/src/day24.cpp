#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  vector<vector<string>> input;
  ll m, n;
  pair<ll, ll> start, goal;
  const map<char, pair<ll, ll>> dir = {
      {'>', {0, 1}}, {'<', {0, -1}}, {'^', {-1, 0}}, {'v', {1, 0}}};
  vector<pair<ll, ll>> goals;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        input.push_back({});
        for (char c : line) {
          if (c != '.') {
            input.back().push_back({c});
          } else {
            input.back().push_back({});
          }
        }
      }
    }

    m = input.size();
    n = input[1].size();
    for (ll j = 0; j < n; ++j) {
      if (input[0][j].size() == 0) {
        start = {0, j};
        input[0][j].push_back('$');
      }
      if (input[m - 1][j].size() == 0) {
        goal = {m - 1, j};
        input[m - 1][j].push_back('$');
      }
    }

    input_file.close();
  }

  void print_grid(const set<tuple<ll, ll, ll>>& dp) {
    for (ll i = 0; i < m; ++i) {
      for (ll j = 0; j < n; ++j) {
        bool seen = false;
        for (ll k = 0; k < (ll)goals.size(); ++k) {
          if (dp.contains({i, j, k})) {
            seen = true;
          }
        }
        if (seen) {
          printf("E");
        } else if (input[i][j].size() == 0) {
          printf(".");
        } else if (input[i][j].size() == 1) {
          printf("%c", input[i][j][0]);
        } else {
          printf("%ld", input[i][j].size() % 10);
        }
      }
      printf("\n");
    }
    printf("\n");
  }

  pair<ll, ll> pmod(const pair<ll, ll>& v) {
    return {(m + (get<0>(v) % m)) % m, (n + (get<1>(v) % n)) % n};
  }

  ll next_goal(ll i, ll j, ll g) {
    if (i == get<0>(goals[g]) && j == get<1>(goals[g])) {
      return g + 1;
    }
    return g;
  }

  void solve1(bool final = false) {
    ll res = 0;
    goals = {goal};
    if (final) {
      goals = {goal, start, goal};
    }

    // dp? blizzards are known.
    // each time step try moving everywhere (at worst x, size of grid states)

    set<tuple<ll, ll, ll>> dp = {{get<0>(start), get<1>(start), 0}};
    tuple<ll, ll, ll> complete = {get<0>(goals.back()), get<1>(goals.back()),
                                  goals.size()};
    ll t = 0;
    // print_grid(dp);

    for (; !dp.contains(complete); t++) {
      vector next_input(m, vector<string>(n));
      for (ll i = 0; i < m; ++i) {
        for (ll j = 0; j < n; ++j) {
          for (char c : input[i][j]) {
            if (c == '#' || c == '$') {
              next_input[i][j].push_back(c);
              continue;
            }
            auto [d_i, d_j] = dir.at(c);
            auto [x, y] = pmod({i + d_i, j + d_j});
            if (input[x][y].size() == 1) {
              if (input[x][y][0] == '#') {
                auto [x1, y1] = pmod({x + d_i, y + d_j});
                if (input[x1][y1].size() != 1 || input[x1][y1][0] != '$') {
                  auto [x2, y2] = pmod({x1 + d_i, y1 + d_j});
                  next_input[x2][y2].push_back(c);
                }
              } else if (input[x][y][0] != '$') {
                next_input[x][y].push_back(c);
              }
            } else {
              next_input[x][y].push_back(c);
            }
          }
        }
      }
      input = next_input;
      set<tuple<ll, ll, ll>> next_dp;
      for (const auto& [i, j, g] : dp) {
        if (input[i][j].size() == 0 || input[i][j][0] == '$') {
          next_dp.insert({i, j, next_goal(i, j, g)});
        }
        for (const auto& [c, d] : dir) {
          if (i == get<0>(start) && j == get<1>(start) && c == '^') {
            continue;
          } else if (i == get<0>(goal) && j == get<1>(goal) && c == 'v') {
            continue;
          }
          const auto& [d_i, d_j] = d;
          ll x = i + d_i, y = j + d_j;
          if (input[x][y].size() == 0 || input[x][y][0] == '$') {
            next_dp.insert({x, y, next_goal(x, y, g)});
          }
        }
      }
      dp = next_dp;

      // printf("time: %lld, dp: %ld\n", t + 1, dp.size());
      // for (auto [i, j] : dp) {
      //   printf("%lld, %lld\n", i, j);
      // }
      // print_grid(dp);
    }
    // print_grid(dp);

    res = t;
    printf("res: %lld\n", res);
  }

  void solve2() { solve1(true); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
