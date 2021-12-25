#include <bits/stdc++.h>
using namespace std;

struct solution {
  using ll = long long;
  vector<int> pos;
  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      line.push_back(',');
      for (size_t i = 0, j = line.find(','); j != std::string::npos;
           i = j + 1, j = line.find(',', i)) {
        if (i < j) {
          pos.emplace_back(stoi(line.substr(i, j - i)));
        }
      }
    }

    input_file.close();
  }

  ll solve1() {
    sort(pos.begin(), pos.end());
    ll dist = accumulate(pos.begin(), pos.end(), 0ll, plus<ll>());
    ll res = dist, sz = pos.size();

    for (int i = 1, mx = pos.back(); i <= mx; ++i) {
      ll l = distance(pos.begin(), lower_bound(pos.begin(), pos.end(), i));
      ll r = sz - l;
      dist += l + r;
      res = min(res, dist);
    }

    printf("res: %lld\n", res);
    return res;
  }

  ll solve2() {
    ll sz = pos.size();
    sort(pos.begin(), pos.end());
    ll dist = accumulate(pos.begin(), pos.end(), 0ll,
                         [](ll acc, int x) { return acc + (x + 1) * x / 2; });
    vector<ll> prefix_sum(sz + 1);
    for (int i = 0; i < sz; ++i) {
      prefix_sum[i + 1] = prefix_sum[i] + pos[i];
    }
    ll res = dist;

    for (int i = 1, mx = pos.back(); i <= mx; ++i) {
      ll l = distance(pos.begin(), lower_bound(pos.begin(), pos.end(), i));
      ll r = sz - l;
      ll l_sum = l * i - (prefix_sum[l] - prefix_sum[0]);
      ll r_sum = prefix_sum[sz] - prefix_sum[l] - r * (i - 1);
      dist += l_sum - r_sum;
      res = min(res, dist);
    }

    printf("res: %lld\n", res);
    return res;
  }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day07.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
