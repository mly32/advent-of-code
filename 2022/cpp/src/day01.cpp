#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct solution {
  vector<vector<int>> listOfNums;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line == "" || listOfNums.empty()) {
        listOfNums.push_back({});
      }
      if (line != "") {
        listOfNums.back().push_back(stoi(line));
      }
    }

    input_file.close();
  }

  void solve1() {
    ll res = 0;

    for (auto& nums : listOfNums) {
      ll sum = 0;
      for (int x : nums) {
        sum += x;
      }
      res = max(res, sum);
    }

    printf("res: %lld\n", res);
  }

  void solve2() {
    priority_queue<int, vector<int>, greater<int>> min_heap;

    for (auto& nums : listOfNums) {
      ll sum = 0;
      for (int x : nums) {
        sum += x;
      }
      min_heap.push(sum);
      if (min_heap.size() > 3) {
        min_heap.pop();
      }
    }

    ll res = 0;
    while (!min_heap.empty()) {
      res += min_heap.top();
      min_heap.pop();
    }
    printf("res: %lld\n", res);
  }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "../input/" + string(final ? "final" : "practice") + "/day" + day + ".in";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
