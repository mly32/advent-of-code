#include <bits/stdc++.h>
using namespace std;

struct solution {
  using ll = long long;
  vector<vector<bool>> bits;
  int sz;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        vector<bool> num(line.size());
        transform(line.begin(), line.end(), num.begin(),
                  [](const char c) { return c == '1'; });

        bits.push_back(num);
      }
    }
    sz = bits[0].size();

    input_file.close();
  }

  ll solve1() {
    vector<vector<int>> cnts = vector<vector<int>>(sz, vector<int>(2));
    for (const vector<bool>& b : bits) {
      for (int i = 0; i < sz; ++i) {
        ++cnts[i][b[i]];
      }
    }
    int gamma = 0, epsilon = 0;

    for (int i = 0; i < sz; ++i) {
      gamma *= 2, epsilon *= 2;
      cnts[i][1] > cnts[i][0] ? ++gamma : ++epsilon;
    }

    ll res = 1ll * gamma * epsilon;
    printf("res: %lld\n", res);
    return res;
  }

  ll common(const bool most) {
    vector<vector<bool>> common_bits = bits;

    ll res = 0;
    for (int i = 0; i < sz; ++i) {
      if (common_bits.size() == 1) {
        res = 2 * res + common_bits[0][i];
        continue;
      }
      int ones = 0, zeros = 0;
      for (const vector<bool>& b : common_bits) {
        b[i] ? ++ones : ++zeros;
      }
      bool keep = most == (ones >= zeros);
      res = 2 * res + keep;

      vector<vector<bool>> nxt_bits;
      for (const vector<bool>& b : common_bits) {
        if (b[i] == keep) {
          nxt_bits.push_back(b);
        }
      }
      common_bits = nxt_bits;
    }
    return res;
  }

  ll solve2() {
    int o2 = common(true), co2 = common(false);
    ll res = 1ll * o2 * co2;
    printf("res: %lld\n", res);
    return res;
  }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day03.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
