#include <bits/stdc++.h>
using namespace std;

struct solution {
  struct hasher {
    std::size_t operator()(const array<int, 5>& k) const {
      static hash<int> h = hash<int>{};
      return h(k[0]) ^ h(k[1]) ^ h(k[2]) ^ h(k[3]) ^ h(k[4]);
    }
  };

  enum op_t : char {
    INP = 'n',
    ADD = 'd',
    MUL = 'u',
    DIV = 'i',
    MOD = 'o',
    EQL = 'q'
  };
  enum var_t { W = 0, X = 1, Y = 2, Z = 3, PC = 4 };
  using val_t = variant<int, var_t>;
  using instr_t = tuple<op_t, val_t, val_t>;
  using reg_t = array<int, 5>;
  using input_t = list<int>;

  vector<instr_t> instructions;
  bool largest;
  int pc_end;
  unordered_map<reg_t, optional<input_t>, hasher> dp;
  static const size_t mx_dp_sz = 1e8;

  solution(const string& filename) {
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
      cerr << "Could not open the file - '" << filename << "'" << endl;
      return;
    }

    while (getline(input_file, line)) {
      if (line != "") {
        instructions.push_back(parse_line(line));
      }
    }
    pc_end = instructions.size();

    input_file.close();
  }

  friend ostream& operator<<(ostream& os, const op_t& op) {
    switch (op) {
      case INP:
        os << "INP";
        return os;
      case ADD:
        os << "ADD";
        return os;
      case MUL:
        os << "MUL";
        return os;
      case DIV:
        os << "DIV";
        return os;
      case MOD:
        os << "MOD";
        return os;
      case EQL:
        os << "EQL";
        return os;
      default:
        return os;
    }
  }

  friend ostream& operator<<(ostream& os, const var_t& var) {
    os << char('w' + var);
    return os;
  }

  instr_t parse_line(const string& line) {
    size_t pos = line.find(' ');
    op_t op{line.substr(0, pos)[1]};

    function<val_t()> read = [&line, &pos]() {
      size_t prv_pos = ++pos;
      pos = line.find(' ', prv_pos);
      string str = line.substr(prv_pos, pos - prv_pos);
      if (islower(str[0])) {
        return val_t{var_t(str[0] - 'w')};
      }
      return val_t{stoi(str)};
    };

    if (op == INP) {
      return {INP, read(), 0};
    }
    return {op_t{op}, read(), read()};
  }

  void print_state(const reg_t& reg) {
    cout << "PC: " << reg[PC] << ", ";
    for (int i = 0; i < 4; ++i) {
      cout << (var_t)i << ": " << reg[i] << ", ";
    }
    cout << endl;
  }

  void print_instructions() {
    for (const auto& [op, a, b] : instructions) {
      cout << op << ":";
      for (const val_t& x : {a, b}) {
        visit([](const auto& e) { cout << " " << e; }, x);
      }
      cout << endl;
    }
  }

  inline int order(const int i) { return largest ? 9 - i : 1 + i; }

  optional<input_t> backtrack(reg_t reg) {
    if (dp.size() > mx_dp_sz) {
      printf("cleared %lu\n", dp.size());
      dp.clear();
    }
    auto [it, unseen] = dp.try_emplace(reg);
    optional<input_t>& res = it->second;
    if (!unseen) {
      return res;
    }
    for (int& pc = reg[PC]; pc < pc_end; ++pc) {
      const auto& [op, a, b] = instructions[pc];
      int& x = reg[get<var_t>(a)];

      if (op == INP) {
        for (int i = 0; i < 9; ++i) {
          x = order(i);
          ++reg[PC];
          if (res = backtrack(reg)) {
            res->push_front(x);
            return res;
          }
          --reg[PC];
        }
      }

      int y = holds_alternative<int>(b) ? get<int>(b) : reg[get<var_t>(b)];
      if (y == 0 && (op == DIV || op == MOD)) {
        reg[Z] = 1;
        break;
      }
      switch (op) {
        case ADD:
          x += y;
          break;
        case MUL:
          x *= y;
          break;
        case DIV:
          x /= y;
          break;
        case MOD:
          x %= y;
          break;
        case EQL:
          x = x == y;
          break;
        default:
          break;
      }
    }
    return reg[Z] == 0 ? make_optional(input_t{}) : nullopt;
  }

  string solve() {
    string res;
    if (optional<input_t> best = backtrack(reg_t{})) {
      for (const int x : *best) {
        res.push_back('0' + x);
      }
    } else {
      res = "none";
    }

    printf("res: %s\n", res.c_str());
    return res;
  }

  string solve1() {
    largest = true;
    return solve();
  }

  string solve2() {
    largest = false;
    return solve();
  }
};

int main() {
  const bool final = 1, snd_question = 0;
  const string filename =
      "./input/" + string(final ? "final" : "practice") + "/day24.txt";
  !snd_question ? solution(filename).solve1() : solution(filename).solve2();
}
