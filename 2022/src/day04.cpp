#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  vector<vector<int>> input;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        vector<int> pos = {-1};
        pos.push_back(line.find('-'));
        pos.push_back(line.find(',', pos.back() + 1));
        pos.push_back(line.find('-', pos.back() + 1));

        vector<int> nums;
        for (int i = 0; i < 4; ++i) {
          nums.push_back(stoi(line.substr(pos[i] + 1, pos[i + 1] - pos[i])));
        }
        input.push_back(nums);
      }
    }
    input_file.close();
  }

  void solve1() {
    ll res = 0;

    for (auto& v : input) {
      int l1 = v[0], r1 = v[1], l2 = v[2], r2 = v[3];
      if ((l1 <= l2 && r2 <= r1) || (l2 <= l1 && r1 <= r2)) {
        res++;
      }
    }

    printf("res: %lld\n", res);
  }

  void solve2() {
    ll res = 0;

    for (auto& v : input) {
      int l1 = v[0], r1 = v[1], l2 = v[2], r2 = v[3];
      if ((l1 <= l2 && l2 <= r1) || (l1 <= r2 && r2 <= r1) ||
          (l2 <= l1 && l1 <= r2) || (l2 <= r1 && r1 <= r2)) {
        res++;
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
