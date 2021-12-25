#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

const int validPos[7] = {0, 1, 3, 5, 7, 9, 10};
const int costs[4] = {1, 10, 100, 1000};
int soln = INT_MAX;

void print(vector<string>& room) {
  for (auto& s : room) {
    cout << s << endl;
  }
  cout << endl;
}

void print(pair<int, int>& p) { cout << p.first << ' ' << p.second << endl; }

vector<string> parseInput() {
  vector<string> room;
  string s;
  getline(cin, s);
  while (getline(cin, s)) room.push_back(s.substr(1, 11));
  room.pop_back();
  return room;
}

vector<string> stks(4), room;

bool hasCollision(int src, int dst) {
  if (src < dst) {
    for (int i = src + 1; i <= dst; i++) {
      if (room[0][i] != '.') return true;
    }
  } else {
    for (int i = dst; i < src; i++) {
      if (room[0][i] != '.') return true;
    }
  }
  return false;
}

int cost = 0, remaining = 0, misplaced[4] = {0};

int getCost(char c, int src, int dst) {
  return abs(src - dst) * costs[c - 'A'];
}

void backtrack() {
  if (cost > soln) return;
  if (remaining == 0) {
    soln = min(soln, cost);
    return;
  }
  // Move out of the rooms
  for (int i = 0; i < stks.size(); i++) {
    if (stks[i].empty()) continue;
    char c = stks[i].back();
    stks[i].pop_back();
    for (auto& j : validPos) {
      if (hasCollision(2 * i + 2, j)) continue;
      cost += getCost(c, 2 * i + 2, j);
      room[0][j] = c;
      backtrack();
      cost -= getCost(c, 2 * i + 2, j);
      room[0][j] = '.';
    }
    stks[i].push_back(c);
  }
  // Move into the rooms
  for (int i = 0; i < room[0].size(); i++) {
    if (room[0][i] == '.') continue;
    char c = room[0][i];
    if (!stks[c - 'A'].empty()) continue;
    if (hasCollision(i, 2 * (c - 'A') + 2)) continue;
    room[0][i] = '.';
    remaining--;
    cost += getCost(c, i, 2 * (c - 'A') + 2);
    backtrack();
    room[0][i] = c;
    remaining++;
    cost -= getCost(c, i, 2 * (c - 'A') + 2);
  }
}

int main() {
  room = parseInput();
  for (int i = 0; i < 4; i++) {
    for (int j = room.size() - 1; j > 0; j--) {
      if (!stks[i].empty() || room[j][2 * i + 2] != i + 'A') {
        stks[i].push_back(room[j][2 * i + 2]);
        // Move out cost
        cost += costs[stks[i].back() - 'A'] * j;
        misplaced[stks[i].back() - 'A']++;
      }
    }
  }
  // Move in cost
  for (int i = 0; i < 4; i++) {
    remaining += misplaced[i];
    cost += costs[i] * misplaced[i] * (misplaced[i] + 1) / 2;
  }
  backtrack();
  cout << soln << endl;
}
