#include <bits/stdc++.h>
using namespace std;

struct solution {
  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      //
    }

    input_file.close();
  }

  int solve1() {
    //
    int res = 0;
    printf("res: %d\n", res);
    return res;
  }

  int solve2() {
    //
    return 0;
  }
};

int main() {
  const bool final = 0, snd_question = 0;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
