#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

int freq[7] = {1, 3, 6, 7, 6, 3, 1};

pair<ll, ll> roll(bool p1turn, ll p1score, ll p2score, int p1pos, int p2pos) {
  pair<ll, ll> soln = {0, 0};
  if (p1score >= 21) {
    return {1, 0};
  }
  if (p2score >= 21) {
    return {0, 1};
  }

  if (p1turn) {
    for (int i = 0; i < 7; i++) {
      int newp1pos = (p1pos + i + 2) % 10 + 1;
      pair<ll, ll> tmp =
          roll(!p1turn, p1score + newp1pos, p2score, newp1pos, p2pos);
      soln.first += tmp.first * freq[i];
      soln.second += tmp.second * freq[i];
    }
  } else {
    for (int i = 0; i < 7; i++) {
      int newp2pos = (p2pos + i + 2) % 10 + 1;
      pair<ll, ll> tmp =
          roll(!p1turn, p1score, p2score + newp2pos, p1pos, newp2pos);
      soln.first += tmp.first * freq[i];
      soln.second += tmp.second * freq[i];
    }
  }
  return soln;
}

int main() {
  string s;
  getline(cin, s);
  ll p1 = s.back() - '0';
  getline(cin, s);
  ll p2 = s.back() - '0';
  bool p1turn = true;
  pair<ll, ll> soln = roll(p1turn, 0, 0, p1, p2);
  cout << max(soln.first, soln.second) << endl;
  cout << soln.first << ' ' << soln.second << endl;
}
