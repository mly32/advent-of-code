#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  vector<string> input;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      input.push_back(line);
    }

    input_file.close();
  }

  void solve1() {
    ll res = 0;

    for (auto& line : input) {
      string s = line.substr(0, line.size() / 2);
      string t = line.substr(line.size() / 2);

      map<char, int> freq_s, freq_t;
      for (auto c : s) {
        freq_s[c]++;
      }
      for (auto c : t) {
        freq_t[c]++;
      }
      for (auto [k, v] : freq_s) {
        if (freq_t[k] > 0) {
          res += islower(k) ? (k - 'a' + 1) : (k - 'A' + 27);
        }
      }
    }

    printf("res: %lld\n", res);
  }

  void solve2() {
    ll res = 0;

    int sz = input.size();
    for (int i = 0; i < sz; i += 3) {
      vector<unordered_map<char, int>> freqs(3);
      for (int j = 0; j < 3; ++j) {
        for (auto c : input[i + j]) {
          freqs[j][c]++;
        }
      }
      for (auto [k, _] : freqs[0]) {
        if (freqs[1][k] > 0 && freqs[2][k] > 0) {
          res += islower(k) ? (k - 'a' + 1) : (k - 'A' + 27);
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
