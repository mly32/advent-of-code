#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  using operation = char;

  struct monkey {
    bool is_x;
    bool yeller;
    ll v;
    ll o1;
    ll o2;
    operation op;
  };

  map<string, ll> ids;
  map<ll, string> monkey_name;
  map<ll, monkey> input;
  map<ll, vector<ll>> signals;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    regex yell_regex("(\\w+): (\\d+)");
    regex wait_regex("(\\w+): (\\w+) (.) (\\w+)");

    while (getline(input_file, line)) {
      smatch match;
      if (regex_match(line, match, yell_regex)) {
        ll m = get_id(match.str(1));
        ll v = stoll(match.str(2));

        input[m] = monkey{false, true, v, 0, 0, 0};
        signals[-1].push_back(m);
      } else if (regex_match(line, match, wait_regex)) {
        ll m = get_id(match.str(1));
        ll o1 = get_id(match.str(2)), o2 = get_id(match.str(4));
        operation op = match.str(3)[0];

        input[m] = monkey{false, false, 0, o1, o2, op};
        signals[o1].push_back(m);
        signals[o2].push_back(m);
      }
    }
    input_file.close();
  }

  ll get_id(const string& s) {
    static ll next_id = 0;
    if (!ids.contains(s)) {
      ids[s] = next_id++;
      monkey_name[ids[s]] = s;
    }
    return ids[s];
  }

  void solve1() {
    ll r = get_id("root");
    map<ll, ll> seen;

    queue<ll> q;
    for (auto m : signals[-1]) {
      q.push(m);
    }

    while (!seen.contains(r)) {
      ll m = q.front();
      q.pop();

      const auto& [is_x, yeller, v, o1, o2, op] = input[m];
      if (yeller) {
        seen[m] = v;
      } else {
        ll x = 0;
        ll v1 = seen[o1], v2 = seen[o2];
        if (op == '+') {
          x = v1 + v2;
        } else if (op == '-') {
          x = v1 - v2;
        } else if (op == '*') {
          x = v1 * v2;
        } else if (op == '/') {
          x = v1 / v2;
        }
        seen[m] = x;
      }

      for (auto n : signals[m]) {
        ll n_o1 = input[n].o1, n_o2 = input[n].o2;
        // cannot be a yeller
        if (seen.contains(n_o1) && seen.contains(n_o2)) {
          q.push(n);
        }
      }
    }

    ll res = seen[r];
    printf("res: %lld\n", res);
  }

  map<ll, ll> deduce_dp;

  pair<bool, ll> deduce(ll m, ll y, bool cont, bool print) {
    if (!print && deduce_dp.contains(m)) {
      return {true, deduce_dp[m]};
    }

    const auto& [is_x, yeller, v, o1, o2, op] = input[m];
    if (is_x) {
      print&& printf("x");
      if (cont) {
        deduce_dp[m] = y;
        return {true, y};
      }
      return {false, 0};
    } else if (yeller) {
      print&& printf("%lld", v);
      deduce_dp[m] = v;
      return {true, v};
    }

    print&& printf("(");
    auto [ok1, v1] = deduce(o1, y, false, print);
    print&& printf(") %c (", op);
    auto [ok2, v2] = deduce(o2, y, false, print);
    print&& printf(")");

    if (ok1 && ok2) {
      ll x = 0;
      if (op == '+') {
        x = v1 + v2;
      } else if (op == '-') {
        x = v1 - v2;
      } else if (op == '*') {
        x = v1 * v2;
      } else if (op == '/') {
        x = v1 / v2;
      }

      deduce_dp[m] = x;
      return {true, x};
    }

    if (!cont) {
      return {false, 0};
    }

    ll x = 0;
    if (ok1) {
      if (op == '+') {
        x = y - v1;
      } else if (op == '-') {
        x = v1 - y;
      } else if (op == '*') {
        x = y / v1;
      } else if (op == '/') {
        x = v1 / y;
      } else if (op == '=') {
        x = v1;
      }
    } else {
      if (op == '+') {
        x = y - v2;
      } else if (op == '-') {
        x = y + v2;
      } else if (op == '*') {
        x = y / v2;
      } else if (op == '/') {
        x = y * v2;
      } else if (op == '=') {
        x = v2;
      }
    }

    print&& printf(" = %lld\n", x);

    ll n = ok1 ? o2 : o1;
    auto res = deduce(n, x, true, print);
    return res;
  }

  void solve2() {
    ll r = get_id("root"), h = get_id("humn");

    input[h] = {true, true, 0, 0, 0, 0};
    input[r].op = '=';

    stringstream ss;

    auto [ok, x] = deduce(r, 0, true, false);

    ll res = x;
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
