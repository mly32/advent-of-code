#include <bits/stdc++.h>

using namespace std;

struct solution {
  enum token { LB = -1, RB = -2, CM = -3 };

  list<list<int>> exprs;
  int i;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        bool is_last_digit = false;
        list<int> expr;
        for (const char c : line) {
          if (isdigit(c)) {
            if (is_last_digit) {
              expr.back() = expr.back() * 10 + c - '0';
            } else {
              expr.push_back(c - '0');
            }
          } else if (c == '[') {
            expr.push_back(LB);
          } else if (c == ']') {
            expr.push_back(RB);
          } else if (c == ',') {
            expr.push_back(CM);
          }
          is_last_digit = isdigit(c);
        }
        exprs.emplace_back(expr);
      }
    }
    input_file.close();
  }

  bool try_explode(list<int>& expr) {
    int depth = 0;
    for (list<int>::iterator it = expr.begin(); it != expr.end(); ++it) {
      if (*it == LB) {
        ++depth;
      } else if (*it == RB) {
        --depth;
      } else if (*it == CM && depth > 4) {
        list<int>::iterator p = prev(it), n = next(it);
        if (*p >= 0 && *n >= 0) {
          list<int>::iterator pp = prev(p), nn = next(n);

          for (list<int>::reverse_iterator l(pp); l != expr.rend(); ++l) {
            if (*l >= 0) {
              *l += *p;
              break;
            }
          }
          for (list<int>::iterator r(nn); r != expr.end(); ++r) {
            if (*r >= 0) {
              *r += *next(it);
              break;
            }
          }

          *it = 0;
          expr.erase(pp), expr.erase(p), expr.erase(nn), expr.erase(n);

          return true;
        }
      }
    }
    return false;
  }

  bool try_split(list<int>& expr) {
    for (list<int>::iterator it = expr.begin(); it != expr.end(); ++it) {
      if (*it >= 10) {
        expr.insert(it, LB);
        expr.insert(it, *it / 2);
        expr.insert(it, CM);
        expr.insert(it, (*it + 1) / 2);
        *it = RB;
        return true;
      }
    }
    return false;
  }

  void reduce(list<int>& expr) {
    bool complete = false;
    do {
      // print(expr, i);
      if (!try_explode(expr)) {
        complete = !try_split(expr);
      }
      ++i;
    } while (!complete);
  }

  bool try_mag(list<int>& expr) {
    for (list<int>::iterator it = expr.begin(); it != expr.end(); ++it) {
      if (*it == CM) {
        list<int>::iterator p = prev(it), n = next(it);
        if (*p >= 0 && *n >= 0) {
          list<int>::iterator pp = prev(p), nn = next(n);

          *it = 3 * (*p) + 2 * (*n);
          expr.erase(pp), expr.erase(p), expr.erase(nn), expr.erase(n);

          return true;
        }
      }
    }
    return false;
  }

  int magnitude(list<int>& expr) {
    i = 0;
    bool complete = false;
    do {
      // print(expr, i);
      complete = !try_mag(expr);
      ++i;
    } while (!complete);

    return expr.front();
  }

  void print(const list<int>& expr, int i = -1) {
    if (i != -1) {
      cout << setw(5) << i << ": ";
    } else {
      cout << "final: ";
    }
    for (const int& x : expr) {
      if (x == LB) {
        cout << "[";
      } else if (x == RB) {
        cout << "]";
      } else if (x == CM) {
        cout << ",";
      } else {
        cout << x;
      }
    }
    cout << endl << endl;
  }

  list<int> sum(const list<int>& a, const list<int>& b) {
    list<int> res;
    res.push_back(LB);
    res.insert(res.end(), a.begin(), a.end());
    res.push_back(CM);
    res.insert(res.end(), b.begin(), b.end());
    res.push_back(RB);

    return res;
  }

  int solve1() {
    int res = 0;
    i = 0;

    list<int> expr;
    list<list<int>>::iterator it = exprs.begin();
    expr = *it;

    for (++it; it != exprs.end(); ++it) {
      expr = sum(expr, *it);
      reduce(expr);
    }

    print(expr);
    cout << "res: " << magnitude(expr) << endl;
    return res;
  }

  int solve2() {
    int res = 0;
    i = 0;

    for (const list<int>& a : exprs) {
      for (const list<int>& b : exprs) {
        if (a != b) {
          list<int> expr = sum(a, b);
          reduce(expr);
          res = max(res, magnitude(expr));
        }
      }
    }

    cout << "res: " << res << endl;
    return res;
  }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day18.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
