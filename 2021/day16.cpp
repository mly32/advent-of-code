#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

struct solution {
  vector<bool> bits;
  ull pos = 0;
  ull total_ver = 0;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    getline(input_file, line);
    bits.resize(line.size() * 4);
    for (int i = line.size() - 1; i >= 0; --i) {
      int h = from_hex(line[i]);
      for (int b = 0; b < 4; ++b) {
        bits[4 * i + b] = (h >> (3 - b)) & 1;
      }
    }

    input_file.close();
  }

  ull from_hex(const char c) { return (c >= 'A') ? (c - 'A' + 10) : (c - '0'); }

  ull read_bits(const int len) {
    ull res = 0;

    for (int i = 0; i < len; ++i) {
      res = 2 * res + bits[pos++];
    }

    return res;
  }

  ull read() {
    total_ver += read_bits(3);
    int t = read_bits(3);

    if (t == 4) {
      ull res = 0;
      while (true) {
        bool cont = read_bits(1);
        res = 16 * res + read_bits(4);

        if (!cont) {
          // cout << "val: " << res << endl;
          return res;
        }
      }
    }

    vector<ull> res;
    int i = read_bits(1);
    if (i) {
      int sub_packets = read_bits(11);
      // cout << "sub: " << sub_packets << endl;
      for (int i = 0; i < sub_packets; ++i) {
        res.push_back(read());
      }
    } else {
      int total_len = read_bits(15);
      // cout << "tot: " << total_len << endl;
      for (ull end_pos = pos + total_len; pos < end_pos;) {
        res.push_back(read());
      }
    }

    switch (t) {
      case 0:
        return accumulate(res.begin(), res.end(), 0llu, plus<ull>());
      case 1:
        return accumulate(res.begin(), res.end(), 1llu, multiplies<ull>());
      case 2:
        return *min_element(res.begin(), res.end());
      case 3:
        return *max_element(res.begin(), res.end());
      case 5:
        return res[0] > res[1];
      case 6:
        return res[0] < res[1];
      case 7:
        return res[0] == res[1];
      default:
        break;
    }

    return 0;
  }

  ull solve() {
    pos = 0;

    ull res = read();

    printf("ver cnt: %llu, res: %llu\n", total_ver, res);
    return res;
  }
};

int main() {
  const bool final = 1;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day16.txt";
  solution(filename).solve();
}
// TODO write to NEVER not define type to constants
