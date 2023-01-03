#include <bits/stdc++.h>

#include <ranges>

using namespace std;

using ll = long long;

vector<ll> largest_n(const vector<ll>& v, size_t n) {
  priority_queue<ll, vector<ll>, greater<ll>> min_heap;

  for (auto& e : v) {
    min_heap.push(e);
    if (min_heap.size() > n) {
      min_heap.pop();
    }
  }

  vector<ll> res;
  while (!min_heap.empty()) {
    res.push_back(min_heap.top());
    min_heap.pop();
  }
  return res;
}

struct solution {
  struct monkey {
    ll id = 0;
    vector<ll> items;
    function<ll(ll)> op;
    ll test = 0;
    pair<ll, ll> jump;
    ll inspection = 0;
  };

  vector<monkey> input;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }
    regex id_regex("Monkey (-?\\d+):");
    regex items_regex("\\s+Starting items: (.+)");
    regex operation_regex("\\s+Operation: new = old (.) (.+)$");
    regex test_regex("\\s+Test: divisible by (.+)$");
    regex jump_regex("\\s+If (.+): throw to monkey (.+)");
    string_view delim = ", ";

    monkey m;

    while (getline(input_file, line)) {
      smatch match;
      if (regex_match(line, match, id_regex)) {
        m.id = stoll(match.str(1));
      } else if (regex_match(line, match, items_regex)) {
        for (const auto& w : string_view(match.str(1)) | views::split(delim)) {
          m.items.push_back(stoll(string(w.begin(), w.end())));
        }
      } else if (regex_match(line, match, operation_regex)) {
        const bool old = match.str(2).compare("old") == 0;
        ll val = old ? 0 : stoll(match.str(2));
        if (match.str(1)[0] == '*') {
          m.op = [=](ll x) { return x * (old ? x : val); };
        } else {
          m.op = [=](ll x) { return x + (old ? x : val); };
        }
      } else if (regex_match(line, match, test_regex)) {
        m.test = stoll(match.str(1));
      } else if (regex_match(line, match, jump_regex)) {
        if (match.str(1).compare("true") == 0) {
          m.jump.first = stoll(match.str(2));
        } else {
          m.jump.second = stoll(match.str(2));
          input.push_back(m);
          m = monkey{};
        }
      }
    }

    input_file.close();
  }

  void solve1(const ll rounds = 20, const ll reduction = 3) {
    auto mod_fold = [](ll acc, monkey& m) { return lcm(acc, m.test); };
    ll mod = accumulate(input.begin(), input.end(), 1ll, mod_fold);
    for (ll r = 0; r < rounds; ++r) {
      for (monkey& m : input) {
        for (ll n : m.items) {
          n = reduction == 1 ? m.op(n) % mod : m.op(n) / reduction;
          ll next_pos = n % m.test == 0 ? m.jump.first : m.jump.second;
          input[next_pos].items.push_back(n);
          m.inspection++;
        }
        m.items.clear();
      }
    }

    vector<ll> v;
    ranges::transform(input, back_inserter(v),
                      [](monkey& m) { return m.inspection; });
    ranges::nth_element(v, v.begin() + 1, greater<ll>());

    ll res = v[0] * v[1];
    printf("res: %lld\n", res);
  }

  void solve2() { solve1(10'000, 1); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
