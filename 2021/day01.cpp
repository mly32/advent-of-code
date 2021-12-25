#include <bits/stdc++.h>
using namespace std;

struct solution {
  vector<int> nums;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        nums.push_back(stoi(line));
      }
    }

    input_file.close();
  }

  int solve1() {
    int res = 0;

    for (int i = nums.size() - 1; i > 0; --i) {
      res += nums[i] > nums[i - 1];
    }

    printf("res: %d\n", res);
    return res;
  }

  int solve2() {
    int res = 0;

    int sz = nums.size(), window = nums[0] + nums[1] + nums[2];

    for (int i = 3; i < sz; ++i) {
      int prv_window = window;
      window += nums[i] - nums[i - 3];
      res += window > prv_window;
    }

    printf("res: %d\n", res);
    return res;
  }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day01.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
