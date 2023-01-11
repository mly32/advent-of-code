#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  valarray<ll> dim;  // { min_x, max_x, min_y, max_y }
  valarray<ll> sand_start{500, 0};
  ll floor = -1;
  unordered_map<ll, set<ll>> init_grid;
  unordered_map<ll, set<ll>> grid;

  void update_dim(ll x, ll y) {
    dim[0] = min(dim[0], x);
    dim[1] = max(dim[1], x);
    dim[2] = min(dim[2], y);
    dim[3] = max(dim[3], y);
  }

  void print_position(const valarray<ll>& pos) {
    for (auto x : pos) {
      printf("%lld,", x);
    }
    printf("\b \b\n");
  }

  void print_grid(bool has_floor = false) {
    for (ll y = dim[2]; y <= dim[3]; ++y) {
      for (ll x = dim[0]; x <= dim[1]; ++x) {
        char c = '.';
        if (init_grid[x].count(y)) {
          c = '#';
        } else if (grid[x].count(y)) {
          c = 'o';
        } else if (x == sand_start[0] && y == sand_start[1]) {
          c = '+';
        }
        printf("%c", c);
      }
      printf("\n");
    }
    if (has_floor) {
      for (ll y = dim[3] + 1; y < floor; ++y) {
        printf("%s\n", string(1 + dim[1] - dim[0], '.').c_str());
      }
      printf("%s\n", string(1 + dim[1] - dim[0], '#').c_str());
    }
  }

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    regex regex("(\\d+),(\\d+)( -> )?");

    dim = {LLONG_MAX, 0, 0, 0};
    vector<vector<valarray<ll>>> input;

    while (getline(input_file, line)) {
      if (line != "") {
        input.push_back({});
        auto match_begin = sregex_iterator(line.begin(), line.end(), regex);
        auto match_end = sregex_iterator();

        for (auto i = match_begin; i != match_end; ++i) {
          ll x = stoll(i->str(1));
          ll y = stoll(i->str(2));

          input.back().push_back({valarray{x, y}});
          update_dim(x, y);
        }
      }
    }
    input_file.close();

    floor = dim[3] + 2;
    init_grid = unordered_map<ll, set<ll>>();

    for (auto& path : input) {
      path.push_back(path.back());
      for (size_t i = 1; i < path.size(); ++i) {
        valarray<ll> delta = (path[i] - path[i - 1]).apply([](ll n) {
          return clamp(n, -1ll, 1ll);
        });
        for (auto pos = path[i - 1]; (pos != path[i]).max(); pos += delta) {
          init_grid[pos[0]].insert(pos[1]);
        }
        init_grid[path[i][0]].insert(path[i][1]);
      }
    }

    grid = init_grid;
  }

  bool move(bool has_floor = false) {
    ll x = sand_start[0], y = sand_start[1];
    if (grid[x].count(y)) {  // not empty
      return false;
    }
    while (true) {
      if (has_floor) {
        for (ll i = x - 1; i <= x + 1; ++i) {  // create floor
          if (grid.find(i) == grid.end() || grid[i].count(floor) == 0) {
            grid[i].insert(floor);
            update_dim(i, y);
          }
        }
      }

      if (grid[x].size() == 0) {  // void
        return false;
      }
      // first filled >= y
      auto it = lower_bound(grid[x].begin(), grid[x].end(), y);
      if (it == grid[x].end()) {  // void
        return false;
      }

      if (y < *it - 1) {
        y = *it - 1;  // move down
      } else if (!grid[x - 1].count(y + 1)) {
        x--, y++;  // move down-left
      } else if (!grid[x + 1].count(y + 1)) {
        x++, y++;  // move down-right
      } else {
        grid[x].insert(y);  // can no longer move
        return true;
      }
    }
  }

  void solve1() {
    ll res = 0;

    while (move()) {
      res++;
    }
    // print_grid();

    printf("res: %lld\n", res);
  }

  void solve2() {
    ll res = 0;

    while (move(true)) {
      res++;
    }
    // print_grid(true);

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
