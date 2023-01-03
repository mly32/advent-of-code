#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const ll MATS = 4;
enum material { ORE = 3, CLAY = 2, OBSIDIAN = 1, GEODE = 0, NONE = 4 };

using mat_array = array<ll, MATS>;
using blueprint = array<mat_array, MATS + 1>;

const map<string, material> material_map = {
    {"ore", ORE},
    {"clay", CLAY},
    {"obsidian", OBSIDIAN},
    {"geode", GEODE},
};

struct solution {
  vector<blueprint> input;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    regex blueprint_regex("Blueprint (\\d+):");
    regex part_regex("\\s+Each (\\w+) robot costs (\\d+) (\\w+).");
    regex parts_regex(
        "\\s+Each (\\w+) robot costs (\\d+) (\\w+) and (\\d+) (\\w+).");

    ll index = -1;

    while (getline(input_file, line)) {
      smatch match;
      if (regex_match(line, match, blueprint_regex)) {
        index = stoll(match.str(1)) - 1;
        input.resize(index + 1);
      } else if (regex_match(line, match, part_regex)) {
        material mat = material_map.at(match.str(1));
        ll p1_cost = stoll(match.str(2));
        material p1_mat = material_map.at(match.str(3));
        input[index][mat][p1_mat] = p1_cost;
      } else if (regex_match(line, match, parts_regex)) {
        material mat = material_map.at(match.str(1));
        ll p1_cost = stoll(match.str(2));
        material p1_mat = material_map.at(match.str(3));
        ll p2_cost = stoll(match.str(4));
        material p2_mat = material_map.at(match.str(5));
        input[index][mat][p1_mat] = p1_cost;
        input[index][mat][p2_mat] = p2_cost;
      }
    }
    input_file.close();
  }

  void print_blueprint(ll b) {
    printf("blueprint %lld >>\n", b + 1);
    for (ll r = 0; r < MATS; ++r) {
      printf("  r %lld: ", r);
      for (ll i = 0; i < MATS; ++i) {
        printf("%lld, ", input[b][r][i]);
      }
      printf("\n");
    }
  }

  void solve1(bool part2 = false) {
    ll res = part2 ? 1 : 0;
    ll max_time = part2 ? 32 : 24;
    if (part2 && input.size() > 3) {
      input.resize(3);
    }

    for (ll b = 0; b < (ll)input.size(); ++b) {
      mat_array max_cost = {0, 0, 0, 0};
      for (ll r = 0; r < MATS; ++r) {
        for (ll i = 0; i < MATS; ++i) {
          max_cost[i] = max(max_cost[i], input[b][r][i]);
        }
      }

      // dp: time -> robot -> resources
      vector<set<pair<mat_array, mat_array>>> dp(max_time + 1);
      dp[0].insert({{0, 0, 0, 1}, {0, 0, 0, 0}});

      auto update = [&](ll t, material robot, mat_array robots,
                        mat_array resources) {
        for (ll i = 0; i < MATS; ++i) {
          resources[i] += robots[i] - input[b][robot][i];
        }
        if (robot != NONE) {
          robots[robot]++;
        }

        for (ll i = 0; i < MATS; ++i) {
          if (i != GEODE) {
            ll cap = max_cost[i] + (max_time - t) * (max_cost[i] - robots[i]);
            resources[i] = clamp(resources[i], 0ll, cap);
          }
        }
        dp[t].insert({robots, resources});
      };
      for (ll t = 0; t < max_time; ++t) {
        for (const auto& [robots, resources] : dp[t]) {
          for (ll r = 0; r < MATS + 1; ++r) {
            bool ok = r == NONE || r == GEODE || robots[r] < max_cost[r];
            for (ll i = 0; i < MATS; ++i) {
              ok &= resources[i] >= input[b][r][i];
            }
            if (!ok) {
              continue;
            }

            update(t + 1, (material)r, robots, resources);
            if (r == GEODE) {  // should always build geode when possible?
              break;
            }
          }
        }

        // printf("  t %lld, next size: %ld\n", t + 1, dp[t + 1].size());
        dp[t] = {};
      }

      ll max_geode = 0;
      for (const auto& [_, resources] : dp[max_time]) {
        max_geode = max(max_geode, resources[GEODE]);
      }
      if (part2) {
        res *= max_geode;
      } else {
        res += (b + 1) * max_geode;
      }
      printf("blueprint %lld max_geodes: %lld\n", b + 1, max_geode);
    }

    printf("res: %lld\n", res);
  }

  void solve2() { solve1(true); }
};

int main(int argc, char** argv) {
  const string day = argc > 1 ? argv[1] : "";
  const bool final = argc > 2 && string(argv[2]) == "1";
  const bool snd_question = argc > 3 && string(argv[3]) == "1";
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day" + day + ".txt";
  snd_question ? solution(filename).solve2() : solution(filename).solve1();
}
