#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  vector<ll> input;
  list<ll> list_input;

  vector<ll> jumps;
  ll zero_v;
  ll n;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        jumps.push_back(stoll(line));
        if (jumps.back() == 0) {
          zero_v = input.size();
        }
        input.push_back(input.size());
        list_input.push_back(list_input.size());
      }
    }
    n = input.size();

    input_file.close();
  }

  void print_input() {
    for (auto i : input) {
      printf("%lld, ", jumps[i]);
    }
    printf("\n");
  }

  ll index(ll i) { return ((i % n) + n) % n; }

  list<ll>::iterator adv(ll amount) {
    auto it = list_input.begin();
    advance(it, amount);
    return it;
  }

  void solve1_list(ll key = 1, ll repeat = 1) {
    ll res = 0;

    for (auto& x : jumps) {
      x *= key;
    }

    for (ll r = 0; r < repeat; ++r) {
      for (ll v = 0; v < n; ++v) {
        auto it = find(list_input.begin(), list_input.end(), v);

        ll x = jumps[v] % (n - 1);
        ll i = distance(list_input.begin(), it);
        ll j = index(i + x);

        if (x > 0) {
          j = index(j + 1);
        }

        auto reposition = j == 0 ? list_input.end() : adv(j);
        list_input.splice(reposition, list_input, it);
      }
    }

    auto it_zero = find(list_input.begin(), list_input.end(), zero_v);
    ll i = distance(list_input.begin(), it_zero);
    printf("zero at: %lld\n", i);
    for (auto x : {1'000ll, 2'000ll, 3'000ll}) {
      ll value = *adv(index(i + x));
      res += jumps[value];
    }

    printf("res: %lld\n", res);
  }

  void solve1(ll key = 1, ll repeat = 1) {
    ll res = 0;

    for (auto& x : jumps) {
      x *= key;
    }

    for (ll r = 0; r < repeat; ++r) {
      for (ll v = 0; v < n; ++v) {
        auto it = find(input.begin(), input.end(), v);

        ll x = jumps[v] % (n - 1);  // problem with old solution: not % n
        ll i = distance(input.begin(), it);

        if (x > 0) {
          for (ll j = i; j < i + x; ++j) {
            swap(input[index(j)], input[index(j + 1)]);
          }
        } else if (x < 0) {
          for (ll j = i; j > i + x; --j) {
            swap(input[index(j)], input[index(j - 1)]);
          }
        }
      }
    }

    auto it_zero = find(input.begin(), input.end(), zero_v);
    ll i = distance(input.begin(), it_zero);
    printf("zero at: %lld\n", i);
    for (auto x : {1'000ll, 2'000ll, 3'000ll}) {
      ll value = input[index(i + x)];
      res += jumps[value];
    }
    printf("res: %lld\n", res);
  }

  void solve2_list() { solve1_list(811'589'153, 10); }

  void solve2() { solve1(811'589'153, 10); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  // snd_question ? solution(filename).solve2() : solution(filename).solve1();
  snd_question ? solution(filename).solve2_list()
               : solution(filename).solve1_list();
}
