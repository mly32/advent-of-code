#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  using point = array<ll, 3>;
  vector<point> input;
  point mn, mx;
  const vector<point> dirs = {{1, 0, 0},  {0, 1, 0},  {0, 0, 1},
                              {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}};

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    regex regex("(\\d+),(\\d+),(\\d+)");
    smatch match;

    mn = {LLONG_MAX, LLONG_MAX, LLONG_MAX};
    mx = {0, 0, 0};

    while (getline(input_file, line)) {
      if (regex_match(line, match, regex)) {
        input.push_back(
            {stoll(match.str(1)), stoll(match.str(2)), stoll(match.str(3))});
        for (int i = 0; i < 3; ++i) {
          mn[i] = min(mn[i], input.back()[i] - 1);
          mx[i] = max(mx[i], input.back()[i] + 1);
        }
      }
    }

    input_file.close();
  }

  void print_point(const point& p) {
    auto [x, y, z] = p;
    printf("%lld,%lld,%lld\n", x, y, z);
  }

  void solve1() {
    ll res = 0;

    set<point> st(input.begin(), input.end());

    for (const auto& [x, y, z] : input) {
      for (const auto& [dx, dy, dz] : dirs) {
        res += !st.contains({x + dx, y + dy, z + dz});
      }
    }

    printf("res: %lld\n", res);
  }

  inline bool all_leq(const point& a, const point& b) {
    return a[0] <= b[0] && a[1] <= b[1] && a[2] <= b[2];
  }

  inline bool all_geq(const point& a, const point& b) {
    return a[0] >= b[0] && a[1] >= b[1] && a[2] >= b[2];
  }

  void solve2() {
    ll res = 0;

    set<point> st(input.begin(), input.end());

    set<point> seen;
    queue<point> q;

    seen.insert(mn);
    q.push(mn);

    // flood fill
    while (!q.empty()) {
      const point p = q.front();
      q.pop();

      for (const auto& [dx, dy, dz] : dirs) {
        point n = {p[0] + dx, p[1] + dy, p[2] + dz};
        if (st.contains(n)) {
          res++;
        } else if (!seen.contains(n) && all_geq(n, mn) && all_leq(n, mx)) {
          seen.insert(n);
          q.push(n);
        }
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
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
