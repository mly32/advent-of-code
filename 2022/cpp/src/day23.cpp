#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  set<pair<ll, ll>> input;
  array<ll, 4> dim;  // mn_i, mn_j, mx_i, mx_j

  enum orientation { N, S, W, E, NE, NW, SE, SW };

  map<orientation, string> name = {
      {N, "N"},   {S, "S"},   {W, "W"},   {E, "E"},
      {NE, "NE"}, {NW, "NW"}, {SE, "SE"}, {SW, "SW"},
  };

  vector<pair<ll, ll>> dirs = {
      {-1, 0},   // N
      {1, 0},    // S
      {0, -1},   // W
      {0, 1},    // E
      {-1, 1},   // NE
      {-1, -1},  // NW
      {1, 1},    // SE
      {1, -1}    // SW
  };

  vector<pair<orientation, vector<orientation>>> checks = {
      {N, {N, NE, NW}},
      {S, {S, SE, SW}},
      {W, {W, NW, SW}},
      {E, {E, NE, SE}},
  };

  void update_dim(ll i, ll j) {
    dim[0] = min(dim[0], i);
    dim[1] = min(dim[1], j);
    dim[2] = max(dim[2], i);
    dim[3] = max(dim[3], j);
  }

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    vector<string> grid;
    while (getline(input_file, line)) {
      if (line != "") {
        grid.push_back(line);
      }
    }

    ll m = grid.size(), n = grid[0].size();
    dim = {LLONG_MAX, LLONG_MAX, LLONG_MIN, LLONG_MIN};
    for (ll i = 0; i < m; ++i) {
      for (ll j = 0; j < n; ++j) {
        if (grid[i][j] == '#') {
          update_dim(i, j);
          input.insert({i, j});
        }
      }
    }

    input_file.close();
  }

  void print_input() {
    for (ll i = dim[0]; i <= dim[2]; ++i) {
      for (ll j = dim[1]; j <= dim[3]; ++j) {
        printf("%c", input.contains({i, j}) ? '#' : '.');
      }
      printf("\n");
    }
    printf("\n");
  }

  void solve1(bool forever = false) {
    ll res = 0;

    ll c_it = 0;

    // print_input();
    ll t = 0;
    for (; forever || t < 10; ++t) {
      // printf("\nc %lld\n", c_it);
      map<pair<ll, ll>, vector<pair<ll, ll>>> proposed;
      set<pair<ll, ll>> next_input;

      for (auto [i, j] : input) {
        if (all_of(dirs.begin(), dirs.end(), [&](pair<ll, ll> d) {
              return !input.contains({i + get<0>(d), j + get<1>(d)});
            })) {
          next_input.insert({i, j});
          // printf("%lld, %lld >> stays\n", i, j);
          continue;
        }
        bool ok = false;
        for (ll k = 0; k < 4; ++k) {
          const auto& [d, c] = checks[(c_it + k) % 4];
          if (all_of(c.begin(), c.end(), [&](orientation o) {
                return !input.contains(
                    {i + get<0>(dirs[o]), j + get<1>(dirs[o])});
              })) {
            proposed[{i + get<0>(dirs[d]), j + get<1>(dirs[d])}].push_back(
                {i, j});
            ok = true;
            break;
          }
        }
        if (!ok) {
          // printf("%lld, %lld >> cannot move, stays\n", i, j);
          next_input.insert({i, j});
        }
      }

      // for (auto [u, l] : proposed) {
      //   printf("%lld, %lld >> ", u.first, u.second);
      //   for (auto [x, y] : l) {
      //     printf("(%lld, %lld), ", x, y);
      //   }
      //   printf("\n");
      // }

      for (const auto& [u, l] : proposed) {
        if (l.size() == 1) {
          next_input.insert(u);
        } else {
          for (const auto& v : l) {
            next_input.insert(v);
          }
        }
      }

      input = next_input;
      dim = {LLONG_MAX, LLONG_MAX, LLONG_MIN, LLONG_MIN};
      for (auto [i, j] : input) {
        update_dim(i, j);
      }

      if (proposed.size() == 0) {
        break;
      }

      c_it = (c_it + 1) % 4;

      // printf("end of round %lld, size: %ld >>\n", t + 1, input.size());
      // print_input();
    }

    if (forever) {
      res = t + 1;
    } else {
      for (ll i = dim[0]; i <= dim[2]; ++i) {
        for (ll j = dim[1]; j <= dim[3]; ++j) {
          res += !input.contains({i, j});
        }
      }
    }

    printf("res: %lld\n", res);
  }

  void solve2() { solve1(true); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "../input/" + string(final ? "final" : "practice") + "/day" + day + ".in";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
