#include <bits/stdc++.h>

using namespace std;
using ull = unsigned long long int;

struct solution {
  int start_pos_a, start_pos_b;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    getline(input_file, line);
    start_pos_a = stoi(line.substr(line.find_first_of(":") + 2));
    getline(input_file, line);
    start_pos_b = stoi(line.substr(line.find_first_of(":") + 2));

    input_file.close();
  }

  int roll() {
    static int k = 0;

    k = (k + 1) % 100;
    return k;
  }

  int move(const int pos, const int mov) {
    static map<pair<int, int>, int> mem;
    if (mem.find({pos, mov}) == mem.end()) {
      mem[{pos, mov}] = ((pos + mov) % 10) ?: 10;
    }

    return mem.at({pos, mov});
  }

  ull solve1() {
    int pos_a = start_pos_a, pos_b = start_pos_b;
    int scr_a = 0, scr_b = 0;

    const int goal = 1000;
    int turns = 0;
    for (bool a = true; scr_a < goal && scr_b < goal; a = !a, ++turns) {
      int mov = roll() + roll() + roll();
      if (a) {
        pos_a = move(pos_a, mov);
        scr_a += pos_a;
        // cout << "a> " << pos_a << " " << scr_a << endl;
      } else {
        pos_b = move(pos_b, mov);
        scr_b += pos_b;
        // cout << "b> " << pos_b << " " << scr_b << endl;
      }
    }
    // cout << 3 * turns << " " << scr_a << " " << scr_b << endl;

    ull res = 3ull * turns * min(scr_a, scr_b);
    cout << "res: " << res << endl;

    return res;
  }

  unordered_map<int, int> die_freq() {
    const int faces = 3;
    unordered_map<int, int> res;

    for (int i = 1; i <= faces; ++i) {
      for (int j = 1; j <= faces; ++j) {
        for (int k = 1; k <= faces; ++k) {
          ++res[i + j + k];
        }
      }
    }

    return res;
  }

  ull solve2() {
    const int max_turns = 16;
    const unordered_map<int, int> freq = die_freq();

    array<array<map<pair<int, int>, ull>, max_turns>, 2> turns;
    array<array<pair<ull, ull>, max_turns>, 2> wins;
    turns[0][0][{start_pos_a, 0}] = 1;
    turns[1][0][{start_pos_b, 0}] = 1;

    const int goal = 21;
    for (int player : {0, 1}) {
      cout << "player " << player << endl;
      for (int i = 0; i < max_turns - 1; ++i) {
        if (turns[player][i].empty()) {
          cout << "break at turn " << i + 1 << endl;
          break;
        }
        for (const auto& [p, cnt] : turns[player][i]) {
          auto [pos, scr] = p;
          for (const auto& [mov, freq] : freq) {
            int nxt_pos = move(pos, mov);
            int nxt_scr = scr + nxt_pos;
            if (nxt_scr >= goal) {
              wins[player][i + 1].first += cnt * freq;
            } else {
              wins[player][i + 1].second += cnt * freq;
              turns[player][i + 1][{nxt_pos, nxt_scr}] += cnt * freq;
            }
          }
        }
      }
    }

    array<ull, 2> total_win = {0, 0};
    for (int player : {0, 1}) {
      cout << "player " << player << endl;
      for (int i = 1; i < max_turns; ++i) {
        cout << setw(3) << i << ": " << setw(16) << wins[player][i].first
             << setw(16) << wins[player][i].second << endl;

        auto [wins_a, left_a] = wins[player][i];
        auto [wins_b, left_b] = player == 0 ? wins[1][i - 1] : wins[0][i];
        total_win[player] += wins_a * left_b;
      }
    }

    ull res = max(total_win[0], total_win[1]);
    cout << "res: " << res << endl;

    return res;
  }
};

int main() {
  const bool final = 1, snd_question = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day21.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
