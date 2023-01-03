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
      if (line != "") {
        input.push_back(line);
      }
    }

    input_file.close();
  }

  void solve1(int count = 4) {
    for (auto& s : input) {
      int res = 0;
      unordered_map<char, int> freq;
      for (int i = 0; i < (int)s.size(); ++i) {
        freq[s[i]]++;
        if (i >= count) {
          freq[s[i - count]]--;
          if (freq[s[i - count]] == 0) {
            freq.erase(s[i - count]);
          }
        }
        int ok = 0;
        for (auto& [k, v] : freq) {
          ok += v == 1;
        }
        if (ok == count) {
          res = i + 1;
          break;
        }
      }
      printf("res: %d\n", res);
    }
  }

  void solve2() { solve1(14); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
