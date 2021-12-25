#include <bits/stdc++.h>
using namespace std;

struct solution {
  array<stack<int>, 11> stk;
  array<int, 11> row;
  array<array<vector<int>, 2>, 11> legal_pos;
  int min_cost = INT_MAX, cost = 0;
  int complete = 0, goal = 0;

  solution(const string& filename) {
    vector<string> lines;
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }
    while (getline(input_file, line)) {
      lines.push_back(line);
    }

    row.fill(0);
    for (int i = lines.size() - 2; i > 1; --i) {
      for (int p = 2; p <= 8; p += 2) {
        int t = 2 * (1 + lines[i][p + 1] - 'A');
        if (!stk[p].empty() || t != p) {
          ++goal;
          stk[p].push(t);
          cost += (i - 1) * (to_cost(t) + to_cost(p));
        }
      }
    }
    cout << "init cost: " << cost << endl;
    legal_pos[0] = {{{0, 1, 3, 5, 7, 9, 10}, {}}};
    legal_pos[2] = {{{1, 0}, {3, 5, 7, 9, 10}}};
    legal_pos[4] = {{{3, 1, 0}, {5, 7, 9, 10}}};
    legal_pos[6] = {{{5, 3, 1, 0}, {7, 9, 10}}};
    legal_pos[8] = {{{7, 5, 3, 1, 0}, {9, 10}}};

    input_file.close();
  }

  inline int to_cost(const int t) {
    return t == 2 ? 1 : t == 4 ? 10 : t == 6 ? 100 : 1000;
  }

  inline int from_cost(const int a, const int b, const int type) {
    return abs(a - b) * to_cost(type);
  }

  inline bool is_movable(const int from, const int to) {
    if (from > to) {
      return is_movable(to, from);
    }
    for (int i = from + 1; i < to; ++i) {
      if (row[i] != 0) {
        return false;
      }
    }
    return true;
  }

  inline void direct_jump(const int p, const int type) {
    int dc = from_cost(p, type, type);
    ++complete, cost += dc;

    if (complete == goal) {
      min_cost = min(min_cost, cost);
    } else {
      backtrack();
    }

    --complete, cost -= dc;
  }

  void inline minor_jump(const int p, const int type) {
    for (const vector<int>& positions : legal_pos[p]) {
      for (const int nxt_pos : positions) {
        if (row[nxt_pos] != 0) {
          break;
        }
        int dc = from_cost(p, nxt_pos, type);
        row[nxt_pos] = type, cost += dc;

        backtrack();

        row[nxt_pos] = 0, cost -= dc;
      }
    }
  }

  void backtrack() {
    if (cost >= min_cost) {
      return;
    }

    for (int p = 8; p >= 2; p -= 2) {
      if (stk[p].empty()) {
        continue;
      }
      int type = stk[p].top();
      stk[p].pop();

      if (stk[type].empty() && is_movable(p, type)) {
        direct_jump(p, type);
      } else {
        minor_jump(p, type);
      }

      stk[p].push(type);
    }

    for (const int i : legal_pos[0][0]) {
      if (row[i] != 0) {
        int p = row[i];
        if (stk[p].empty() && is_movable(i, p)) {
          row[i] = 0;
          direct_jump(i, p);
          row[i] = p;
        }
      }
    }
  }

  int solve() {
    backtrack();

    int res = min_cost;

    printf("res: %d\n", res);
    return res;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day23.txt";
  solution(filename).solve();
}
