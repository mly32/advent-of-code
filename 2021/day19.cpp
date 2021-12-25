#include <bits/stdc++.h>

using namespace std;

struct solution {
  using v = array<int, 3>;
  using m = array<array<int, 3>, 3>;
  using scanner = vector<v>;
  using mapping = function<v(const v&)>;

  v cross_product(const v& a, const v& b) {
    return {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    };
  }

  v mult(const m& mat, const v& a) {
    return {
        mat[0][0] * a[0] + mat[0][1] * a[1] + mat[0][2] * a[2],
        mat[1][0] * a[0] + mat[1][1] * a[1] + mat[1][2] * a[2],
        mat[2][0] * a[0] + mat[2][1] * a[1] + mat[2][2] * a[2],
    };
  }

  v neg(const v& a) { return {-a[0], -a[1], -a[2]}; }

  v plu(const v& a, const v& b) {
    return {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
  }

  int manhattan(const v& a, const v& b) {
    return abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2]);
  }

  vector<m> get_orientations() {
    vector<v> basis = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

    vector<m> res;
    for (v a : basis) {
      for (v b : basis) {
        if (a != b) {
          res.push_back({a, b, cross_product(a, b)});
          res.push_back({neg(a), b, cross_product(neg(a), b)});
          res.push_back({a, neg(b), cross_product(a, neg(b))});
          res.push_back({neg(a), neg(b), cross_product(neg(a), neg(b))});
        }
      }
    }
    return res;
  }

  vector<m> orientations;

  vector<scanner> input_scanners;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        if (line[1] == '-') {
          input_scanners.push_back({});
        } else {
          size_t c1 = line.find(',');
          size_t c2 = line.find(',', c1 + 1);
          int relX = stoi(line.substr(0, c1));
          int relY = stoi(line.substr(c1 + 1, c2 - c1));
          int relZ = stoi(line.substr(c2 + 1));
          input_scanners.back().push_back({relX, relY, relZ});
        }
      }
    }

    input_file.close();
  }

  mapping get_mapping(const m& orientation, const v& a, const v& b) {
    const v shift = plu(b, neg(mult(orientation, a)));
    return [=](const v& x) { return plu(mult(orientation, x), shift); };
  }

  pair<bool, mapping> find_mapping(const scanner& a, const scanner& b) {
    int a_sz = a.size(), b_sz = b.size();
    set<v> b_set(b.begin(), b.end());

    for (int i = 0; i < a_sz; ++i) {
      for (int j = 0; j < b_sz; ++j) {
        for (m& orientation : orientations) {
          mapping f = get_mapping(orientation, a[i], b[j]);
          int common = 0;
          for (const v& x : a) {
            common += b_set.count(f(x));
          }
          if (common >= 12) {
            return {true, f};
          }
        }
      }
    }

    return {false, [](const v& x) { return x; }};
  }

  void solve() {
    orientations = get_orientations();

    int s_sz = input_scanners.size();

    queue<int> q;
    vector<bool> complete(s_sz);

    vector<v> scanners = {{0, 0, 0}};
    // solution relative to scanner 0
    q.push(0);
    complete[0] = true;

    // graph is connected
    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (int i = 0; i < s_sz; ++i) {
        if (!complete[i]) {
          auto [ok, f] = find_mapping(input_scanners[i], input_scanners[u]);
          if (ok) {
            for (v& x : input_scanners[i]) {
              x = f(x);
            }
            scanners.push_back(f({0, 0, 0}));
            q.push(i);
            complete[i] = true;
          }
        }
      }
    }

    set<v> beacons;
    for (int i = 0; i < s_sz; ++i) {
      beacons.insert(input_scanners[i].begin(), input_scanners[i].end());
    }

    int max_manhattan = 0;
    for (int i = 0; i < s_sz; ++i) {
      for (int j = 0; j < s_sz; ++j) {
        max_manhattan = max(max_manhattan, manhattan(scanners[i], scanners[j]));
      }
    }

    printf("beacons: %lu, max manhattan: %d\n", beacons.size(), max_manhattan);
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day19.txt";
  solution(filename).solve();
}
