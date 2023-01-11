#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct solution {
  vector<string> input;
  using inode = vector<tuple<bool, string, ll>>;  // is dir, name, size

  map<vector<string>, inode> fs;
  map<vector<string>, ll> sz;

  ll dfs(vector<string>& loc) {
    auto& info = fs[loc];
    ll res = 0;
    for (auto [is_dir, name, size] : info) {
      if (is_dir) {
        loc.push_back(name);
        res += dfs(loc);
        loc.pop_back();
      } else {
        res += size;
      }
    }
    sz[loc] = res;
    return res;
  }

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        input.push_back(line);
      }
    }

    create_fs();

    input_file.close();
  }

  void create_fs() {
    input.push_back("$ cd /");
    vector<string> path = {""};
    inode info;

    smatch match;
    for (auto& line : input) {
      if (line[0] == '$' && info.size() > 0) {
        fs[path] = info;
        info.clear();
      }

      if (line == "$ ls") {
        // do nothing
      } else if (regex_match(line, match, regex("\\$ cd (.+)"))) {
        string dir = match.str(1);
        if (dir == "/") {
          path = {""};
        } else if (dir == "..") {
          path.pop_back();
        } else {
          path.push_back(dir);
        }
      } else if (regex_match(line, match, regex("dir (.+)"))) {
        info.push_back({true, match.str(1), 0});
      } else if (regex_match(line, match, regex("(.+) (.+)"))) {
        info.push_back({false, match.str(2), stoll(match.str(1))});
      }
    }

    vector<string> loc = {""};
    dfs(loc);
  }

  string pathname(const vector<string>& a) {
    string res = "";
    for (auto& s : a) {
      if (res.size() == 1) {
        res += s;
      } else {
        res += "/" + s;
      }
    }
    return res;
  }

  void print_fs() {
    for (auto& [k, i] : fs) {
      for (auto& [is_dir, name, size] : i) {
        printf("%-10s > %c %-10s %lld\n", pathname(k).c_str(),
               is_dir ? '.' : ' ', name.c_str(), size);
      }
    }
  }

  void solve1() {
    ll res = 0;

    print_fs();

    for (auto& [k, size] : sz) {
      if (size <= 100'000) {
        res += size;
      }
    }

    printf("res: %lld\n", res);
  }

  void solve2() {
    ll total_space = 70000000;
    ll req = 30000000;
    ll unused = total_space - sz[{""}];
    ll need_free = max(0ll, req - unused);

    vector<ll> lengths;
    for (auto& [k, size] : sz) {
      lengths.push_back(size);
    }
    sort(lengths.begin(), lengths.end());

    ll res = *lower_bound(lengths.begin(), lengths.end(), need_free);

    printf("res: %lld\n", res);
  }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "../input/" + string(final ? "final" : "practice") + "/day" + day + ".in";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
