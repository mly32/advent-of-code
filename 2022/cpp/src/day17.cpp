#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  static const ll row_width = 9;
  using shape_t = vector<bitset<row_width>>;
  const ll shapes = 5;
  vector<shape_t> input;
  shape_t chamber;
  string wind;
  const bitset<row_width> empty = bitset<row_width>(0b1'0000'0001);
  map<tuple<ll, ll, ll>, bitset<row_width>> shifted;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    getline(input_file, wind);

    while (getline(input_file, line)) {
      if (line != "") {
        if (input.empty()) {
          input.push_back({});
        }
        bitset<row_width> bs;
        for (size_t i = 0; i < line.size(); ++i) {
          bs[i] = line[i] == '#';
        }
        input.back().push_back(bs);
      } else {
        if ((ll)input.size() == shapes) {
          break;
        }
        input.push_back({});
      }
    }

    for (auto& r : input) {
      reverse(r.begin(), r.end());
    }

    input_file.close();
  }

  void print_v(const shape_t& v, ll limit = 15, bool newline = false) {
    ll l = max(0ll, (ll)v.size() - limit);
    for (ll i = v.size() - 1; i >= l; --i) {
      for (ll j = 0; j < row_width; ++j) {
        printf("%c", v[i][j] ? '#' : '.');
      }
      printf("\n");
    }
    if (newline) {
      printf("\n");
    }
  }

  const bitset<row_width>& get_shifted(ll i, ll j, ll x) {
    if (shifted.find({i, j, x}) == shifted.end()) {
      shifted[{i, j, x}] = x >= 0 ? (input[i][j] << x) : (input[i][j] >> -x);
    }
    return shifted[{i, j, x}];
  }

  void solve1(ll its = 2022) {
    ll res = -1;
    ll height = 1;
    chamber.push_back(bitset<row_width>().set());

    vector<vector<shape_t>> rocks;
    for (ll k = 0; k < row_width; ++k) {
      rocks.push_back(input);
      for (ll i = 0; i < shapes; ++i) {
        for (ll j = 0; j < (ll)rocks[k][i].size(); ++j) {
          rocks[k][i][j] <<= k;
        }
      }
    }

    map<pair<ll, ll>, array<ll, 6>> history;

    const ll resize_at = 10'000, keep = 10;

    ll total_jump = 0;

    for (ll it = 0, w = 0, i = 0; it < its; ++it, i = (i + 1) % shapes) {
      if (chamber.size() > resize_at) {
        res += height - keep;
        shape_t next_chamber;
        for (ll j = height - keep; j < height; ++j) {
          next_chamber.push_back(chamber[j]);
        }
        chamber = next_chamber;
        chamber.reserve(resize_at);
        height = keep;
      }

      const auto& r = input[i];
      ll real_x = 3, real_y = height + 3;

      bool move_down = false;
      while (true) {
        ll dx = 0, dy = 0;
        if (move_down) {
          dy = -1;
        } else {
          dx = wind[w] == '>' ? 1 : -1;
          w = (w + 1) % wind.size();
        }

        ll x = real_x + dx, y = real_y + dy;

        bool collided = false;
        for (ll j = 0, cur_y = y; j < (ll)r.size(); ++j, ++cur_y) {
          const bitset<row_width>& chamber_row =
              (ll)chamber.size() <= cur_y ? empty : chamber[cur_y];
          if ((chamber_row & rocks[x][i][j]).any()) {
            collided = true;
            break;
          }
        }

        if (collided && move_down) {
          for (ll j = 0, cur_y = real_y; j < (ll)r.size(); ++j, ++cur_y) {
            while ((ll)chamber.size() <= cur_y) {
              chamber.push_back(empty);
            }
            chamber[cur_y] |= rocks[real_x][i][j];
          }

          height = max(height, real_y + (ll)r.size());
          break;
        }

        if (!collided) {
          real_x += dx, real_y += dy;
        }
        move_down = !move_down;
      }

      if (i == 0 && total_jump == 0) {  // on flat rock pattern, jump
        auto [h1, h2, h3, it1, it2, it3] = history[{i, w}];
        ll h = res + height;
        if (h - h1 == h1 - h2 && h1 - h2 == h2 - h3 &&  //
            it - it1 == it1 - it2 && it1 - it2 == it2 - it3) {
          ll height_gap = h - h1, it_gap = it - it1;
          ll jumpable_rocks = (its - it) / it_gap;
          ll leftover = (its - it) % it_gap;

          total_jump = jumpable_rocks * height_gap;
          if (leftover == 0) {
            total_jump--;  // compensate
          }
          printf("jumping from %lld to %lld, running %lld extra\n", it,
                 its - leftover, leftover);
          its = it + leftover;  // continue simulation of leftover
        }
        history[{i, w}] = {h, h1, h2, it, it1, it2};
      }
    }

    res += height + total_jump;
    printf("res: %lld\n", res);
  }

  void solve2() { solve1(1'000'000'000'000); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "../input/" + string(final ? "final" : "practice") + "/day" + day + ".in";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
