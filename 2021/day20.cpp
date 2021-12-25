#include <bits/stdc++.h>

using namespace std;

struct solution {
  vector<bool> mp;
  vector<vector<bool>> img;
  bool unseen_state;
  size_t m;
  size_t n;

  vector<bool> convert_str(const string& line) {
    vector<bool> res(line.size());
    for (int i = line.size() - 1; i >= 0; --i) {
      res[i] = line[i] == '#';
    }
    return res;
  }

  size_t flatten(const size_t i, const size_t j) {
    size_t res = 0;
    for (int di : {-1, 0, 1}) {
      for (int dj : {-1, 0, 1}) {
        res = 2 * res + img[i + di][j + dj];
      }
    }
    return res;
  }

  void resize() {
    vector<vector<bool>> res(m + 4, vector<bool>(n + 4, unseen_state));
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        res[i + 2][j + 2] = img[i][j];
      }
    }

    m = res.size(), n = res[0].size();
    img = res;
  }

  void update() {
    vector<vector<bool>> res(m - 2, vector<bool>(n - 2));
    for (size_t i = 1; i < m - 1; ++i) {
      for (size_t j = 1; j < n - 1; ++j) {
        res[i - 1][j - 1] = mp[flatten(i, j)];
      }
    }

    unseen_state = mp[unseen_state ? 0b111'111'111 : 0b0];
    m = res.size(), n = res[0].size();
    img = res;
  }

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }
    getline(input_file, line);
    mp = convert_str(line);
    while (getline(input_file, line)) {
      if (line != "") {
        img.push_back(convert_str(line));
      }
    }
    input_file.close();

    unseen_state = 0;
    m = img.size(), n = img[0].size();
    resize();
  }

  int solve(const size_t times) {
    size_t res = 0;
    // print();

    for (size_t i = 0; i < times; ++i) {
      update();
      resize();
      // print();
    }

    res = 0;
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        res += img[i][j];
      }
    }
    cout << "res: " << res << endl;

    return res;
  }

  int solve1() { return solve(2); }

  int solve2() { return solve(50); }

  void print() {
    cout << m << "x" << n << endl;
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        cout << (img[i][j] ? "#" : ".");
      }
      cout << endl;
    }
    cout << "---" << endl;
  }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day20.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
