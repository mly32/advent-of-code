#include <bits/stdc++.h>
using namespace std;

struct solution {
  vector<pair<int, int>> steps;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        size_t s = line.find(' ');
        int n = stoi(line.substr(s + 1));
        if (line[0] == 'f') {
          steps.push_back({n, 0});
        } else if (line[0] == 'd') {
          steps.push_back({0, n});
        } else {
          steps.push_back({0, -n});
        }
      }
    }

    input_file.close();
  }

  int solve1() {
    int x = 0, y = 0;

    for (const auto& [dx, dy] : steps) {
      x += dx, y += dy;
    }

    int res = x * y;
    printf("res: %d\n", res);
    return res;
  }

  int solve2() {
    int x = 0, y = 0, aim = 0;

    for (const auto& [dx, dy] : steps) {
      x += dx, aim += dy;
      y += aim * dx;
    }

    int res = x * y;
    printf("res: %d\n", res);
    return res;
  }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day02.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
