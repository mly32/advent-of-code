#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  array<ll, 4> dim;            // {min_x, min_y, max_x, max_y }
  vector<array<ll, 3>> input;  // {x, y, dist}
  set<pair<ll, ll>> sensors;
  set<pair<ll, ll>> beacons;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    regex regex(".* x=(.+), y=(.+): .*x=(.+), y=(.+)");
    smatch match;

    dim = {LLONG_MAX, LLONG_MAX, 0, 0};

    while (getline(input_file, line)) {
      if (regex_match(line, match, regex)) {
        array<ll, 4> v = {stoll(match.str(1)), stoll(match.str(2)),
                          stoll(match.str(3)), stoll(match.str(4))};
        update_dim(v);
        sensors.insert({v[0], v[1]});
        beacons.insert({v[2], v[3]});
        input.push_back({v[0], v[1], dist(v)});
      }
    }

    input_file.close();
  }

  void print_position(const array<ll, 4>& v) {
    printf("%lld,%lld ~ %lld,%lld\n", v[0], v[1], v[2], v[3]);
  }

  void update_dim(const array<ll, 4>& v) {
    dim[0] = min({dim[0], v[0], v[2]});
    dim[1] = min({dim[1], v[1], v[3]});
    dim[2] = max({dim[2], v[0], v[2]});
    dim[3] = max({dim[3], v[1], v[3]});
  }

  ll dist(ll x1, ll y1, ll x2, ll y2) { return abs(x1 - x2) + abs(y1 - y2); }
  ll dist(const array<ll, 4>& v) { return dist(v[0], v[1], v[2], v[3]); }

  bool not_beacon(ll x, ll y) {
    if (sensors.count({x, y})) {
      return true;
    }
    if (beacons.count({x, y})) {
      return false;
    }
    for (const auto& a : input) {
      auto [x1, y1, d] = a;
      if (dist(x, y, x1, y1) <= d) {
        return true;
      }
    }
    return false;
  }

  void solve1(ll y) {
    ll res = 0;

    print_position(dim);
    const ll worst_dist = dist(dim);
    for (ll x = dim[0] - worst_dist; x <= dim[2] + worst_dist; ++x) {
      res += not_beacon(x, y);
    }

    printf("res: %lld\n", res);
  }

  void solve2(const ll sz) {
    print_position(dim);

    vector<vector<pair<ll, bool>>> bounds(sz + 1);
    for (const auto& [x1, y1, d] : input) {
      for (ll x = max(0ll, x1 - d); x <= min(sz, x1 + d); ++x) {
        ll d_y = d - abs(x - x1);
        bounds[x].push_back({y1 - d_y, 0});
        bounds[x].push_back({y1 + d_y, 1});
      }
    }

    ll y = -1;
    for (ll x = 0; x <= sz; ++x) {
      auto& b = bounds[x];
      ranges::sort(b);
      if (b[0].first > 0) {
        y = b[0].first - 1;
      } else if (b.back().first < sz) {
        y = b.back().first + 1;
      } else {
        ll depth = 0;
        for (size_t i = 0; i < b.size() - 1; ++i) {
          depth += b[i].second ? -1 : 1;

          if (depth == 0 && b[i].first + 1 < b[i + 1].first) {
            y = b[i].first + 1;
            break;
          }
        }
      }
      if (y != -1) {
        ll res = x * 4'000'000 + y;
        printf("%lld, %lld\n", x, y);
        printf("res: %lld\n", res);
        break;
      }
    }
  }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2(final ? 4'000'000 : 20)
               : solution(filename).solve1(final ? 2'000'000 : 10);
}
