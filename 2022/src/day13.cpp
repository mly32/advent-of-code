#include <bits/stdc++.h>

using namespace std;

using ll = long long;

class packet {
 public:
  packet(bool is_list, vector<packet> list, ll value)
      : is_list(is_list), list(list), value(value) {}

  packet() : packet(true, {}, 0) {}

  packet(vector<packet> list) : packet(true, list, 0) {}

  packet(ll v, ll depth = 0) : is_list(depth > 0) {
    if (is_list) {
      list.push_back(packet(v, depth - 1));
      value = 0;
    } else {
      value = v;
    }
  }

  vector<packet>& get_list() { return list; }

  friend bool operator<(const packet& a, const packet& b) {
    return a.right_order(b) == 1;
  }

  void print(bool newline = true) const {
    if (is_list) {
      printf("[");
      for (auto& e : list) {
        e.print(false);
        printf(",");
      }
      if (!list.empty()) {
        printf("\b");
      }
      printf("]");
    } else {
      printf("%lld", value);
    }
    if (newline) {
      printf("\n");
    }
  }

 private:
  bool is_list;
  vector<packet> list;
  ll value;

  // -1: false, 0: continue, 1: true
  ll right_order(const packet& p) const {
    if (!is_list && !p.is_list) {
      return clamp(p.value - value, -1ll, 1ll);
    } else if (is_list && p.is_list) {
      for (size_t i = 0; i < list.size(); ++i) {
        if (i >= p.list.size()) {
          return -1;
        }
        ll res = list[i].right_order(p.list[i]);
        if (res != 0) {
          return res;
        }
      }
      return list.size() < p.list.size();
    } else if (!is_list) {
      return packet(vector{*this}).right_order(p);
    } else if (!p.is_list) {
      return this->right_order(packet(vector{p}));
    }
    return 0;
  }
};

struct solution {
  vector<packet> input;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        stack<packet> stk;
        stk.push(packet());
        for (size_t pos = 0; pos < line.size();) {
          size_t next_pos = line.find_first_of("[],", pos);
          if (pos != next_pos) {
            ll value = stoll(line.substr(pos, next_pos - pos));
            stk.top().get_list().push_back(packet(value));
          }
          if (line[next_pos] == '[') {
            stk.push(packet());
          } else if (line[next_pos] == ']') {
            packet p = stk.top();
            stk.pop();
            stk.top().get_list().push_back(p);
          }
          pos = next_pos + 1;
        }
        input.push_back(stk.top().get_list()[0]);
      }
    }

    input_file.close();
  }

  void solve1() {
    ll res = 0;
    for (size_t i = 0; i < input.size(); i += 2) {
      if (input[i] < input[i + 1]) {
        res += 1 + i / 2;
      }
    }
    printf("res: %lld\n", res);
  }

  void solve2() {
    sort(input.begin(), input.end());
    auto it1 = lower_bound(input.begin(), input.end(), packet(2, 2));
    auto it2 = lower_bound(input.begin(), input.end(), packet(6, 2));
    ll res =
        (distance(input.begin(), it1) + 1) * (distance(input.begin(), it2) + 2);
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
