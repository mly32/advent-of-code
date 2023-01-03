#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  vector<string> input;

  const map<char, ll> to_dec = {
      {'2', 2}, {'1', 1}, {'0', 0}, {'-', -1}, {'=', -2}};
  const map<ll, char> to_snafu = {
      {2, '2'}, {1, '1'}, {0, '0'}, {-1, '-'}, {-2, '='}};

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

  string from_dec(ll n) {
    if (n == 0) {
      return "0";
    }
    string res;
    for (; n != 0;) {
      ll place = n % 5;
      n /= 5;
      if (place > 2) {
        place -= 5;
        n++;
      }
      res.push_back(to_snafu.at(place));
    }
    reverse(res.begin(), res.end());
    return res;
  }

  void solve1() {
    ll total = 0;
    for (const string& s : input) {
      ll v = 0;

      for (ll i = s.size() - 1, p = 1; i >= 0; --i, p *= 5) {
        v += p * to_dec.at(s[i]);
      }

      total += v;
    }

    string res = from_dec(total);
    printf("res: %s\n", res.c_str());
  }

  void solve2() { solve1(); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
