#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using pll = pair<ll, ll>;

struct solution {
  ll flat_nodes = 0;
  vector<ll> flat_flows;
  vector<string> node_str;
  vector<list<pair<ll, ll>>> flat_g;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    regex line_regex("Valve (..) .*=(.+); .* valve(s?) (.*)");
    regex neighbors_regex("(..)(, )?");
    smatch match;

    set<ll> nodes;
    map<ll, ll> flows;
    map<ll, list<pair<ll, ll>>> g;

    while (getline(input_file, line)) {
      if (regex_match(line, match, line_regex)) {
        ll u = (match.str(1)[0] - 'A') * 26 + match.str(1)[1] - 'A';
        nodes.insert(u);
        flows[u] = stoll(match.str(2));
        const string& v = match.str(match.size() - 1);
        auto match_begin = sregex_iterator(v.begin(), v.end(), neighbors_regex);
        auto match_end = sregex_iterator();

        for (auto i = match_begin; i != match_end; ++i) {
          ll v = (i->str(1)[0] - 'A') * 26 + i->str(1)[1] - 'A';
          g[u].push_back({v, 1});
        }
      }
    }
    input_file.close();

    map<ll, ll> to_flat_nodes;

    for (ll node : nodes) {
      if (node == 0 || flows[node] != 0) {
        to_flat_nodes[node] = flat_nodes;
        flat_flows.push_back(flows[node]);

        string s(' ', 2);
        s[0] = (node / 26) + 'A';
        s[1] = (node % 26) + 'A';
        node_str.push_back(s);

        flat_nodes++;
      }
    }

    flat_g.resize(flat_nodes);

    for (ll node : nodes) {
      if (node == 0 || flows[node] != 0) {
        map<ll, ll> dist;
        priority_queue<pll, vector<pll>, greater<pll>> pq;

        dist[node] = 0;
        pq.push({0, node});

        while (!pq.empty()) {
          auto [d, u] = pq.top();
          pq.pop();
          for (auto [v, _] : g[u]) {
            if (dist.find(v) == dist.end() || dist[v] > dist[u] + 1) {
              dist[v] = dist[u] + 1;
              pq.push({dist[v], v});
            }
          }
        }
        for (auto [u, d] : dist) {
          if (node != u && (u == 0 || flows[u] != 0)) {
            flat_g[to_flat_nodes[node]].push_back({to_flat_nodes[u], d});
          }
        }
      }
    }
  }

  void print_graph() {
    for (ll u = 0; u < flat_nodes; ++u) {
      printf("%s [label=\"%s: %lld\"]\n", node_str[u].c_str(),
             node_str[u].c_str(), flat_flows[u]);
    }
    for (ll u = 0; u < flat_nodes; ++u) {
      for (auto [v, d] : flat_g[u]) {
        if (u < v) {
          printf("%s -- %s [len=%lld] // label=%lld\n", node_str[u].c_str(),
                 node_str[v].c_str(), d, d);
        }
      }
    }
  }

  vector<bool> make_seen(const vector<bool>& seen, initializer_list<ll> list) {
    vector<bool> next_seen = seen;
    for (auto u : list) {
      next_seen[u] = true;
    }
    return next_seen;
  }

  void solve1() {
    ll res = 0;
    print_graph();

    // location, seen, time, pressure
    vector<vector<tuple<vector<bool>, ll, ll>>> dp(flat_nodes);
    ll size = 1;
    dp[0].emplace_back(make_seen(vector<bool>(flat_nodes), {0}), 30, 0);

    while (size > 0) {
      vector<vector<tuple<vector<bool>, ll, ll>>> next_dp(flat_nodes);
      for (ll u = 0; u < flat_nodes; ++u) {
        for (const auto& [seen, time, pressure] : dp[u]) {
          for (auto [v, d] : flat_g[u]) {
            ll next_time = time - d - 1;
            if (!seen[v] && next_time > 0) {
              ll next_pressure = pressure + next_time * flat_flows[v];
              next_dp[v].emplace_back(make_seen(seen, {v}), next_time,
                                      next_pressure);
              res = max(res, next_pressure);
            }
          }
        }
      }
      dp = next_dp;
      size = 0;
      for (ll u = 0; u < flat_nodes; ++u) {
        size += next_dp[u].size();
      }
    }

    printf("res: %lld\n", res);
  }

  void solve2() {
    ll res = 0;

    // seen -> loc1, time1, loc2, time2 -> max pressure
    map<vector<bool>, map<array<ll, 4>, ll>> dp;
    dp[make_seen(vector<bool>(flat_nodes), {0})][{0, 26, 0, 26}] = 0;

    for (ll old_res = -1; old_res != res;) {
      old_res = res;
      map<vector<bool>, map<array<ll, 4>, ll>> next_dp;
      for (const auto& [seen, info] : dp) {
        for (const auto& [k, pressure] : info) {
          const auto& [u1, t1, u2, t2] = k;
          for (auto [v1, d1] : flat_g[u1]) {
            ll next_t1 = max(0ll, t1 - d1 - 1);
            for (auto [v2, d2] : flat_g[u2]) {
              ll next_t2 = max(0ll, t2 - d2 - 1);
              if (v1 != v2) {
                ll next_pressure = pressure;
                array<ll, 4> next_k = k;
                if (!seen[v1] && next_t1 > 0) {
                  next_pressure += next_t1 * flat_flows[v1];
                  next_k[0] = v1, next_k[1] = next_t1;
                }
                if (!seen[v2] && next_t2 > 0) {
                  next_pressure += next_t2 * flat_flows[v2];
                  next_k[2] = v2, next_k[3] = next_t2;
                }

                auto& mp = next_dp[make_seen(seen, {v1, v2})];
                if (next_pressure > old_res &&  // heuristic
                    next_pressure > mp[next_k]) {
                  mp[next_k] = next_pressure;
                  res = max(res, next_pressure);
                }
              }
            }
          }
        }
      }
      dp = next_dp;
    }
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
