#include <bits/stdc++.h>
using namespace std;

struct solution {
  list<char> polymer;
  map<pair<char, char>, size_t> pairs;
  map<pair<char, char>, char> rules;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }
    getline(input_file, line);
    polymer = list<char>(line.begin(), line.end());

    while (getline(input_file, line)) {
      if (line == "") {
        continue;
      }
      rules[{line[0], line[1]}] = line.back();
    }
    for (auto it = next(polymer.begin()); it != polymer.end(); ++it) {
      ++pairs[{*prev(it), *it}];
    }

    input_file.close();
  }

  size_t solve1() {
    const int steps = 10;
    for (int i = 0; i < steps; ++i) {
      cout << setw(3) << "i: " << polymer.size() << endl;
      for (auto it = next(polymer.begin()); it != polymer.end(); ++it) {
        if (rules.find({*prev(it), *it}) == rules.end()) {
          continue;
        }
        polymer.insert(it, rules.at({*prev(it), *it}));
      }
    }

    map<char, size_t> freqs;

    for (const auto& c : polymer) {
      ++freqs[c];
    }
    auto [mn, mx] = minmax_element(
        freqs.begin(), freqs.end(),
        [](const pair<char, size_t>& a, const pair<char, size_t>& b) {
          return a.second < b.second;
        });
    size_t res = mx->second - mn->second;
    printf("res: %lu\n", res);
    return res;
  }

  size_t solve2() {
    const int steps = 40;
    map<char, size_t> freqs;
    for (const auto& c : polymer) {
      ++freqs[c];
    }

    for (int i = 0; i < steps; ++i) {
      // for (const auto& [p, cnt] : pairs) {
      //   cout << cnt << " ";
      //   printf("%c%c: %lu\n", p.first, p.second, cnt);
      // }
      // cout << endl;
      map<pair<char, char>, size_t> nxt_pairs;

      for (const auto& [p, cnt] : pairs) {
        if (rules.find(p) == rules.end()) {
          nxt_pairs[p] += cnt;
        } else {
          char c = rules.at(p);
          nxt_pairs[{p.first, c}] += cnt;
          nxt_pairs[{c, p.second}] += cnt;
          freqs[c] += cnt;
        }
      }

      // pairs.insert(nxt_pairs.begin(), nxt_pairs.end());
      pairs = nxt_pairs;
    }

    auto [mn, mx] = minmax_element(
        freqs.begin(), freqs.end(),
        [](const pair<char, size_t>& a, const pair<char, size_t>& b) {
          return a.second < b.second;
        });
    size_t res = mx->second - mn->second;
    printf("res: %lu\n", res);
    return res;
  }
};

int main() {
  const bool final = 0, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day14.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
