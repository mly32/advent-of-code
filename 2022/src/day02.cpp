#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  const vector<int> result = {0, 3, 6};
  const vector<int> score = {1, 2, 3};
  vector<pair<int, int>> input;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line.size() == 3) {
        input.push_back({line[0] - 'A', line[2] - 'X'});
      }
    }

    input_file.close();
  }

  void solve1() {
    ll res = 0;

    for (auto& [opp, cur] : input) {
      if ((opp + 1) % 3 == cur) {
        res += 6;
      } else if (opp == cur) {
        res += 3;
      }
      res += score[cur];
    }

    printf("res: %lld\n", res);
  }

  void solve2() {
    ll res = 0;

    for (auto& [opp, cur] : input) {
      res += result[cur];

      if (cur == 2) {
        res += score[(opp + 1) % 3];
      } else if (cur == 1) {
        res += score[opp];
      } else {
        res += score[(3 + opp - 1) % 3];
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
