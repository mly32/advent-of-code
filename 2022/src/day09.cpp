#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  map<char, valarray<ll>> dirs = {
      {'U', {1, 0}},
      {'D', {-1, 0}},
      {'L', {0, -1}},
      {'R', {0, 1}},
  };
  vector<pair<char, ll>> input;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      smatch match;
      if (regex_match(line, match, regex("(\\w+) (\\d+)"))) {
        input.push_back({match.str(1)[0], stoll(match.str(2))});
      }
    }

    input_file.close();
  }

  void solve1(ll knots = 2) {
    const valarray<ll> threshold = {1, 1};
    vector<valarray<ll>> rope(knots, {0, 0});

    auto valarray_cmp = []<typename T>(const valarray<T>& a,
                                       const valarray<T>& b) {
      if (a.size() != b.size()) {
        return a.size() < b.size();
      }
      for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
          return a[i] < b[i];
        }
      }
      return false;
    };

    set<valarray<ll>, decltype(valarray_cmp)> seen;
    seen.insert({0, 0});

    for (auto [d, len] : input) {
      for ([[maybe_unused]] auto _ : views::iota(0ll, len)) {
        rope[0] += dirs.at(d);
        // adjacent_difference(
        //     begin(rope), end(rope), begin(rope),
        //     [&](auto b, auto a) -> valarray<ll> {
        //       auto diff = a - b;
        //       if ((abs(diff) > threshold).max()) {
        //         return b + diff.apply([](ll n) { return clamp(n, -1ll, 1ll);
        //         });
        //       }
        //       return b;
        //     });
        for (ll i = 1; i < knots; ++i) {
          auto diff = rope[i - 1] - rope[i];
          if ((abs(diff) > threshold).max()) {
            rope[i] += diff.apply([](ll n) { return clamp(n, -1ll, 1ll); });
          }
        }
        seen.insert(rope.back());
      }
    }

    ll res = seen.size();
    printf("res: %lld\n", res);
  }

  void solve2() { solve1(10); };
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
