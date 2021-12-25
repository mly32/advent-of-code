#include <bits/stdc++.h>
using namespace std;

struct solution {
  using ull = unsigned long long;
  vector<int> ages;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      line.push_back(',');
      for (size_t i = 0, j = line.find(','); j != std::string::npos;
           i = j + 1, j = line.find(',', i)) {
        if (i < j) {
          ages.emplace_back(stoi(line.substr(i, j - i)));
        }
      }

      input_file.close();
    }
  }

  ull solve(const int days) {
    array<ull, 9> cnts{};

    for (const int a : ages) {
      ++cnts[a];
    }

    for (int d = 0; d < days; ++d) {
      array<ull, 9> nxt_cnts{};
      nxt_cnts[6] += cnts[0], nxt_cnts[8] += cnts[0];
      for (int i = 1; i < 9; ++i) {
        nxt_cnts[i - 1] += cnts[i];
      }
      cnts = nxt_cnts;
    }

    ull res = accumulate(cnts.begin(), cnts.end(), 0ull, plus<ull>());
    printf("res: %llu\n", res);
    return res;
  }

  ull solve1() { return solve(80); }

  ull solve2() { return solve(256); }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day06.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
