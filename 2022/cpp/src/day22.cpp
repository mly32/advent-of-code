#include <bits/stdc++.h>

using namespace std;

using ll = long long;

enum facing { R = 0, D = 1, L = 2, U = 3 };
const vector<ll> dirs = {0, 1, 0, -1, 0};
const map<char, facing> to_facing = {{'R', R}, {'D', D}, {'L', L}, {'U', U}};
const map<facing, char> from_facing = {{R, '>'}, {D, 'v'}, {L, '<'}, {U, '^'}};

struct solution {
  ll m, n;
  vector<string> input;
  vector<pair<ll, facing>> moves;
  map<pair<ll, ll>, facing> seen;

  facing f;
  ll x, y;

  vector<pair<ll, ll>> row_bounds;
  vector<pair<ll, ll>> col_bounds;

  ll dim;
  map<ll, pair<ll, ll>> to_pos;
  vector<vector<ll>> face;
  map<pair<ll, facing>, pair<ll, facing>> edges;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line) && line != "") {
      input.push_back(line);
    }

    m = input.size(), n = 0;
    for (auto& row : input) {
      n = max(n, (ll)row.size());
    }
    row_bounds.resize(m, {LLONG_MAX, 0});
    col_bounds.resize(n, {LLONG_MAX, 0});

    for (ll i = 0; i < m; ++i) {
      input[i].resize(n, ' ');
      for (ll j = 0; j < n; ++j) {
        if (input[i][j] != ' ') {
          get<0>(row_bounds[i]) = min(get<0>(row_bounds[i]), j);
          get<1>(row_bounds[i]) = max(get<1>(row_bounds[i]), j);
          get<0>(col_bounds[j]) = min(get<0>(col_bounds[j]), i);
          get<1>(col_bounds[j]) = max(get<1>(col_bounds[j]), i);
        }
      }
    }

    getline(input_file, line);
    regex grid_regex("(\\d+)([RDLU])?");
    auto match_begin = sregex_iterator(line.begin(), line.end(), grid_regex);
    auto match_end = sregex_iterator();
    for (auto i = match_begin; i != match_end; ++i) {
      if (i->str(2).size() == 0) {
        moves.push_back({stoll(i->str(1)), U});
        break;
      }
      moves.push_back({stoll(i->str(1)), to_facing.at(i->str(2)[0])});
    }

    dim = LLONG_MAX;
    for (ll i = 0; i < m; ++i) {
      dim = min(dim, get<1>(row_bounds[i]) - get<0>(row_bounds[i]) + 1);
    }
    for (ll j = 0; j < n; ++j) {
      dim = min(dim, get<1>(col_bounds[j]) - get<0>(col_bounds[j]) + 1);
    }
    printf("dim %lld\n", dim);
    face = vector(m, vector<ll>(n, -1));

    // TODO generate graph automatically
    // edge alg ??? traverse bfs + pq starting with all

    regex node_regex("(\\d+): (\\d+),(\\d+)");
    regex edge_regex("(\\d+)([RDLU])--(\\d+)([RDLU])");

    while (getline(input_file, line)) {
      smatch match;
      if (regex_match(line, match, node_regex)) {
        ll id = stoll(match.str(1));
        ll scaled_i = stoll(match.str(2)), scaled_j = stoll(match.str(3));
        to_pos[id] = {scaled_i, scaled_j};
        for (ll i = scaled_i * dim; i < (scaled_i + 1) * dim; ++i) {
          for (ll j = scaled_j * dim; j < (scaled_j + 1) * dim; ++j) {
            face[i][j] = id;
          }
        }
      } else if (regex_match(line, match, edge_regex)) {
        auto e1 = make_pair(stoll(match.str(1)), to_facing.at(match.str(2)[0]));
        auto e2 = make_pair(stoll(match.str(3)), to_facing.at(match.str(4)[0]));

        if (edges.contains(e2) && edges[e2] != e1) {
          throw invalid_argument("bad edge");
        }
        edges[e1] = e2, edges[e2] = e1;
      }
    }

    input_file.close();
  }

  void print_grid() {
    for (ll i = 0; i < m; ++i) {
      for (ll j = 0; j < n; ++j) {
        if (seen.contains({i, j})) {
          printf("%c", from_facing.at(seen[{i, j}]));
        } else {
          printf("%c", input[i][j]);
        }
      }
      printf("\n");
    }
  }

  bool move() {
    ll i = x + dirs[f], j = y + dirs[f + 1];
    if (i < 0 || i >= m || j < 0 || j >= n || input[i][j] == ' ') {
      if (f == R) {
        j = get<0>(row_bounds[i]);
      } else if (f == D) {
        i = get<0>(col_bounds[j]);
      } else if (f == L) {
        j = get<1>(row_bounds[i]);
      } else {
        i = get<1>(col_bounds[j]);
      }
    }

    if (input[i][j] == '#') {
      return false;
    }
    x = i, y = j;
    return true;
  }

  ll pmod(ll v, ll w) { return (w + (v % w)) % w; }

  tuple<ll, ll, facing> cube_rotate(ll x, ll y, facing f1, facing f2) {
    facing rot_f = facing(pmod(2 + f2, 4));
    ll diff = pmod((ll)rot_f - (ll)f1, 4);
    ll rel_x = pmod(x, dim), rel_y = pmod(y, dim);

    ll rot_x = 0, rot_y = 0;
    if (diff == 0) {
      rot_x = rel_x, rot_y = rel_y;
    } else if (diff == 1) {
      rot_x = rel_y, rot_y = pmod(dim - 1 - rel_x, dim);
    } else if (diff == 2) {
      rot_x = pmod(dim - 1 - rel_x, dim), rot_y = pmod(dim - 1 - rel_y, dim);
    } else if (diff == 3) {
      rot_x = pmod(dim - 1 - rel_y, dim), rot_y = rel_x;
    }

    return {rot_x, rot_y, rot_f};
  }

  bool move_cube() {
    ll i = x + dirs[f], j = y + dirs[f + 1];
    facing next_f = f;
    if (i < 0 || i >= m || j < 0 || j >= n || face[i][j] != face[x][y]) {
      auto [id2, f2] = edges[{face[x][y], f}];

      auto [rot_x, rot_y, rot_f] = cube_rotate(x, y, f, f2);
      next_f = rot_f;
      // printf("rot (%lld, %lld), %c\n", rot_x, rot_y, from_facing.at(f));
      ll rel_i = pmod(rot_x + dirs[rot_f], dim),
         rel_j = pmod(rot_y + dirs[rot_f + 1], dim);
      // printf("rel (%lld, %lld), %c\n", rel_i, rel_j, from_facing.at(rot_f));

      auto [f2_i, f2_j] = to_pos[id2];
      // printf("abs (%lld, %lld)\n", dim * f2_i, dim * f2_j);
      i = f2_i * dim + rel_i, j = f2_j * dim + rel_j;

      // printf("moving from (%lld, %lld) to (%lld, %lld)\n", x, y, i, j);
    }

    if (input[i][j] == '#') {
      return false;
    }
    f = next_f;
    x = i, y = j;
    return true;
  }

  facing rotate(facing g, facing d_g) {
    ll diff = 0;
    if (d_g == R) {
      diff = 1;
    } else if (d_g == L) {
      diff = -1;
    }
    return facing(pmod((ll)g + diff, 4));
  }

  void solve1(bool is_cube = false) {
    f = R, x = 0, y = input[0].find('.');
    seen[{x, y}] = f;
    for (auto [forward, d] : moves) {
      // printf("forward %lld %c\n  then turn %c, next_f %c\n", forward,
      //        from_facing.at(f), d == R ? 'R' : 'L',
      //        from_facing.at(rotate(f, d)));

      for (ll i = 0; i < forward; ++i) {
        if (is_cube ? !move_cube() : !move()) {
          break;
        }
        seen[{x, y}] = f;
      }
      f = rotate(f, d);
      seen[{x, y}] = f;

      // print_grid();
      // printf("\n\n");
    }

    printf("%lld, %lld, %c\n", y + 1, x + 1, from_facing.at(f));
    ll res = 1'000ll * (x + 1) + 4ll * (y + 1) + f;
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
