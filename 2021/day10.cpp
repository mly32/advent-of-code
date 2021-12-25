#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

struct solution {
  vector<string> lines;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      lines.push_back(line);
    }

    input_file.close();
  }

  inline bool is_closing(const char c) {
    return c == ')' || c == ']' || c == '}' || c == '>';
  }

  inline bool is_matching(const char b, const char c) {
    return (b == '(' && c == ')') || (b == '[' && c == ']') ||
           (b == '{' && c == '}') || (b == '<' && c == '>');
  }

  ull solve() {
    const map<char, int> points = {{'(', 1},    {'[', 2},    {'{', 3},
                                   {'<', 4},    {')', 3},    {']', 57},
                                   {'}', 1197}, {'>', 25137}};
    ull res = 0;
    vector<ull> scores;

    for (const string& line : lines) {
      ull score_corrupt = 0, score_incomplete = 0;
      stack<char> stk;

      for (char c : line) {
        if (!is_closing(c)) {
          stk.push(c);
          continue;
        }
        if (stk.empty()) {
          score_corrupt += points.at(c);
          break;
        } else if (is_matching(stk.top(), c)) {
          stk.pop();
        } else {
          score_corrupt += points.at(c);
          break;
        }
      }
      if (score_corrupt == 0) {
        while (!stk.empty()) {
          score_incomplete = 5 * score_incomplete + points.at(stk.top());
          stk.pop();
        }
        scores.push_back(score_incomplete);
      }

      res += score_corrupt;
    }

    sort(scores.begin(), scores.end());

    printf("res: %llu, %llu\n", res, scores[scores.size() / 2]);
    return res;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day10.txt";
  solution(filename).solve();
}
