#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

const int HASH = 256;

int main() {
  string s;
  int rows = 0, cols = 0, moved = 0, steps = 0;
  unordered_set<int> down, right;
  while (getline(cin, s)) {
    cols = s.size();
    for (int i = 0; i < cols; i++) {
      if (s[i] == '.') continue;
      if (s[i] == 'v')
        down.insert((rows << 8) + i);
      else
        right.insert((rows << 8) + i);
    }
    rows++;
  }
  do {
    moved = 0;
    vector<pair<int, int>> newDown, newRight;
    int newVal;
    for (const auto& it : right) {
      newVal = it + 1;
      if ((newVal & (HASH - 1)) == cols) newVal &= ~(HASH - 1);
      if (down.find(newVal) == down.end() &&
          right.find(newVal) == right.end()) {
        newRight.push_back({it, newVal});
        moved++;
      }
    }
    for (const auto& [before, after] : newRight) {
      right.erase(before);
      right.insert(after);
    }
    for (const auto& it : down) {
      newVal = it + HASH;
      if (newVal >= (rows << 8)) newVal &= HASH - 1;
      if (down.find(newVal) == down.end() &&
          right.find(newVal) == right.end()) {
        newDown.push_back({it, newVal});
        moved++;
      }
    }
    for (const auto& [before, after] : newDown) {
      down.erase(before);
      down.insert(after);
    }
    steps++;
  } while (moved);

  cout << steps << endl;
}
