#include <bits/stdc++.h>
using namespace std;

struct solution {
  map<string, set<string>> adj;
  map<string, int> max_visits;
  map<string, int> seen;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      size_t dash = line.find_first_of('-');
      string u = line.substr(0, dash);
      string v = line.substr(dash + 1);
      // if (u == "start" || u == "end") {
      //   transform(u.begin(), u.end(), u.begin(), ::toupper);
      // }
      // if (v == "start" || v == "end") {
      //   transform(v.begin(), v.end(), v.begin(), ::toupper);
      // }
      adj[u].insert(v);
      adj[v].insert(u);
    }

    for (const auto& [u, neighbors] : adj) {
      max_visits[u] = is_small(u) ? 1 : INT_MAX;
    }
    max_visits["start"] = 1, max_visits["end"] = 1;

    input_file.close();
  }

  bool is_small(const string& u) { return islower(u[0]); }

  int dfs(const string& u) {
    if (u == "end") {
      return 1;
    }
    int res = 0;
    ++seen[u];
    for (const string& v : adj[u]) {
      if (seen[v] < max_visits[v]) {
        res += dfs(v);
      }
    }
    --seen[u];
    return res;
  }

  int solve1() {
    seen.clear();
    int res = dfs("start");

    printf("res: %d\n", res);
    return res;
  }

  int solve2() {
    int base = solve1(), res = base;

    for (const auto& [u, neighbors] : adj) {
      if (is_small(u) && u != "start" && u != "end") {
        ++max_visits[u];
        res += solve1() - base;
        --max_visits[u];
      }
      //   if (islower(u[0])
      //     adj = original;
      //     seen.clear();
      //     string u_prime = u + "__";
      //     for (const string& v : neighbors) {
      //       adj[u_prime].insert(v);
      //       adj[v].insert(u_prime);
      //     }
      //     int res = dfs("start") - base;
      //     printf("res: %d\n", res);
      //   }
    }
    printf("res: %d\n", res);

    return 0;
  }
};

int main() {
  const bool final = 1, snd_question = 2;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day12.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
