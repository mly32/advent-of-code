#include <bits/stdc++.h>
using namespace std;

struct solution {
  using ll = long long;
  using cuboid_t = array<int, 6>;
  vector<pair<cuboid_t, bool>> steps;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line == "") {
        continue;
      }

      cuboid_t cuboid;
      for (int i = 0, pos = 0; i < 3; ++i) {
        size_t x1_start = line.find_first_of("=", pos) + 1;
        size_t x1_end = line.find_first_of(".", x1_start);
        cuboid[2 * i] = stoi(line.substr(x1_start, x1_end - x1_start));

        size_t x2_start = x1_end + 2;
        size_t x2_end = line.find_first_of(",", x2_start);
        cuboid[2 * i + 1] = stoi(line.substr(x2_start, x2_end - x2_start));

        pos = x2_end;
      }
      steps.emplace_back(cuboid, line[1] == 'n');
    }

    input_file.close();
  }

  pair<bool, cuboid_t> intersect(const cuboid_t& a, const cuboid_t& b) {
    cuboid_t in;
    for (int i = 0; i < 3; ++i) {
      in[2 * i] = max(a[2 * i], b[2 * i]);
      in[2 * i + 1] = min(a[2 * i + 1], b[2 * i + 1]);
    }

    return {in[0] <= in[1] && in[2] <= in[3] && in[4] <= in[5], in};
  }

  vector<pair<cuboid_t, bool>> cap(const vector<pair<cuboid_t, bool>>& a) {
    vector<pair<cuboid_t, bool>> res;
    cuboid_t range = {-50, 50, -50, 50, -50, 50};
    for (auto& [cub, light] : a) {
      auto [valid, in] = intersect(range, cub);
      if (valid) {
        res.push_back({in, light});
      }
    }
    return res;
  }

  ll solve1() {
    steps = cap(steps);
    return solve2();
  }

  ll solve2() {
    vector<pair<cuboid_t, bool>> cuboids;

    for (const auto& [cub, light] : steps) {
      int sz = cuboids.size();
      if (light) {
        cuboids.push_back({cub, light});
      }
      // negate intersection cuboids
      for (int i = 0; i < sz; ++i) {
        const auto& [c, l] = cuboids[i];
        auto [valid, in] = intersect(c, cub);
        if (valid) {
          cuboids.push_back({in, !l});
        }
      }
    }

    ll res = 0;
    for (const auto& [c, l] : cuboids) {
      ll sgn = (l ? 1 : -1);
      res += sgn * (c[1] - c[0] + 1) * (c[3] - c[2] + 1) * (c[5] - c[4] + 1);
    }

    printf("res: %lld\n", res);
    return res;
  }
};

int main() {
  const bool final = 0, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day22.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
