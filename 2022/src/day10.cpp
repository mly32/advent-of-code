#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  vector<pair<ll, ll>> input;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        smatch match;
        if (regex_match(line, match, regex("addx (-?\\d+)"))) {
          input.push_back({2, stoll(match.str(1))});
        } else {
          input.push_back({1, 0});
        }
      }
    }

    input_file.close();
  }

  void solve1() {
    ll res = 0;

    ll t = 0, x = 1;
    for (auto [dt, dx] : input) {
      while (dt > 0) {
        t++, dt--;
        if (t % 40 == 20) {
          res += t * x;
        }
      }
      x += dx;
    }

    printf("res: %lld\n", res);
  }

  void solve2() {
    string res;

    ll t = 0, x = 1;
    for (auto [dt, dx] : input) {
      while (dt > 0) {
        t++, dt--;
        ll pos = (t - 1) % 40;
        res.push_back(x - 1 <= pos && pos <= x + 1 ? '#' : ' ');
      }
      x += dx;
    }

    for (size_t i = 0; i < res.size(); i += 40) {
      printf("%s\n", res.substr(i, 40).c_str());
    }
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
