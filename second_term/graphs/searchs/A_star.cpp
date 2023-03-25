#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void Print(std::vector<int> v) {
  for (int i = 0; i < 9; ++i) {
    std::cout << v[i] << ' ';
    if (i % 3 == 2) {
      std::cout << '\n';
    }
  }
  std::cout << '\n';
}

uint64_t Hash(std::vector<int>& field) {
  uint64_t hash = 0;
  uint64_t pow = 1;
  for (int i = 0; i < 9; ++i) {
    hash += field[i] * pow;
    pow <<= 4;
  }
  return hash;
}

std::vector<int> Unhash(uint64_t hash) {
  std::vector<int> field;
  field.reserve(9);
  uint64_t pow = 1;
  for (int i = 0; i < 8; ++i) {
    field.push_back((hash % (pow << 4)) / pow);
    pow <<= 4;
  }
  field.push_back(hash / pow);
  return field;
}

int GetNum(uint64_t pos, uint64_t hash) {
  uint64_t pow = 1;
  pow <<= (4 * pos);
  if (pos == 8) {
    return hash / pow;
  }
  return (hash % (pow << 4)) / pow;
}

class State {
 public:
  State(uint64_t hash, int zero_pos) : hash(hash), zero_pos(zero_pos) {}
  uint64_t hash;
  std::vector<State> GetNeighbours() {
    std::vector<State> states;
    if (zero_pos >= 3) {
      states.push_back(Swap(zero_pos - 3));
    }
    if (zero_pos <= 5) {
      states.push_back(Swap(zero_pos + 3));
    }
    if (zero_pos % 3 != 0) {
      states.push_back(Swap(zero_pos - 1));
    }
    if (zero_pos % 3 != 2) {
      states.push_back(Swap(zero_pos + 1));
    }
    return states;
  }
  bool operator==(State& s) { return hash == s.hash; }

  int zero_pos;

  State Swap(int to) {
    uint64_t value = GetNum(to, hash);
    uint64_t new_hash = hash;
    uint64_t pow = 1;
    new_hash -= value * (pow << (4 * to));
    new_hash += value * (pow << (4 * zero_pos));
    State n_state(new_hash, to);
    return n_state;
  }
};

bool operator==(const State& a, const State& b) { return a.hash == b.hash; }

class AbstractHeuristic {
 public:
  virtual double operator()(const State&) = 0;
};

class ConvexCombination : public AbstractHeuristic {
 public:
  ConvexCombination(const std::vector<AbstractHeuristic*>& heuristics,
                    const std::vector<double>& lambdas)
      : heuristics_(heuristics), lambdas_(lambdas) {}

  double operator()(const State& state) {
    double res = 0.;
    for (size_t i = 0; i < heuristics_.size(); ++i) {
      res += heuristics_[i]->operator()(state) * lambdas_[i];
    }
    return res;
  }

 private:
  std::vector<AbstractHeuristic*> heuristics_;
  std::vector<double> lambdas_;
};

class EmptyManhattan : public AbstractHeuristic {
 public:
  double operator()(const State& state) {
    return 4 - state.zero_pos / 3 + state.zero_pos % 3;
  }
};

class CornerManhattan : public AbstractHeuristic {
 public:
  double operator()(const State& state) {
    int dist = 0;
    uint64_t has = state.hash;
    for (int i = 0; i < 9; ++i) {
      if (GetNum(i, has) == 1 || GetNum(i, has) == 3 || GetNum(i, has) == 7) {
        dist += 4 - i % 3 - i / 3;
      }
    }
    return dist;
  }
};

class SumManhattan : public AbstractHeuristic {
 public:
  double operator()(const State& state) {
    int dist = 0;
    uint64_t has = state.hash;
    for (int i = 0; i < 9; ++i) {
      if (i != state.zero_pos && GetNum(i, has) != i + 1) {
        dist += 4 - i % 3 - i / 3;
      }
    }
    return dist;
  }
};

class MismatchesHeuristic : public AbstractHeuristic {
 public:
  double operator()(const State& state) {
    double mism = 0;
    uint64_t has = state.hash;
    for (int i = 0; i < 9; ++i) {
      if (GetNum(i, has) != i + 1 && GetNum(i, has) != 0) {
        ++mism;
      }
    }
    return mism;
  }
};

static constexpr size_t kInfty = std::numeric_limits<size_t>::max();

template <>
struct std::hash<State> {
  std::size_t operator()(State const& s) const noexcept {
    auto hasher = std::hash<uint64_t>{};
    size_t hash = hasher(s.hash);
    return hash;
  }
};

class Visitor {
 public:
  void PushVertex(State state, int move) { ancestors_[state] = move; }
  std::unordered_map<State, int> GetAncestors() { return ancestors_; }

 private:
  std::unordered_map<State, int> ancestors_;
};

struct Compare {
  bool operator()(std::pair<double, State>& a, std::pair<double, State>& b) {
    return a.first > b.first;
  }
};

int AStar(State& from, State& to, ConvexCombination heuristic,
          Visitor& visitor) {
  std::unordered_map<State, size_t> distances;
  std::unordered_map<State, double> eur;
  distances[from] = 0;

  std::priority_queue<std::pair<double, State>,
                      std::vector<std::pair<double, State>>, Compare>
      queue;

  queue.emplace(distances[from] + heuristic(from), from);

  while (!queue.empty()) {
    auto[dist, now_near] = queue.top();
    queue.pop();

    if (now_near.hash == to.hash) {
      return distances[now_near];
    }
    for (auto& i : now_near.GetNeighbours()) {
      if (distances.find(i) == distances.end()) {
        distances[i] = kInfty;
      }

      size_t dist = distances[now_near] + 1;
      if (dist < distances[i]) {
        distances[i] = dist;
        visitor.PushVertex(i, i.zero_pos - now_near.zero_pos);
        queue.emplace(distances[i] + heuristic(i), i);
      }
    }
  }
  return -1;
}

std::vector<int> GetPath(State& from, State& to,
                         std::unordered_map<State, int>& ancs) {
  State pos = from;
  int zero_pos = from.zero_pos;
  std::vector<int> moves;
  while (pos.hash != to.hash) {
    int move = ancs[pos];
    moves.push_back(move);
    pos = pos.Swap(zero_pos - move);
    zero_pos -= move;
  }
  return moves;
}

int main() {
  std::vector<int> field;
  int zero_pos = 0;
  for (size_t i = 0; i < 9; ++i) {
    int temp;
    std::cin >> temp;
    field.push_back(temp);
    if (temp == 0) {
      zero_pos = i;
    }
  }
  State state(Hash(field), zero_pos);
  for (size_t i = 0; i < 8; ++i) {
    field[i] = i + 1;
  }
  field[8] = 0;
  zero_pos = 8;
  State to(Hash(field), zero_pos);
  auto* m_h = new MismatchesHeuristic();
  auto* e_h = new EmptyManhattan();
  ConvexCombination comb({m_h, e_h}, {0.5, 0.5});
  Visitor visitor;
  int answ = AStar(state, to, comb, visitor);

  std::cout << answ;
  if (answ == -1) {
    return 0;
  }
  std::cout << '\n';
  std::unordered_map<State, int> ancs = visitor.GetAncestors();

  std::vector<int> path = GetPath(to, state, ancs);
  for (size_t i = path.size(); i > 0; --i) {
    if (path[i - 1] % 3 == 0) {
      std::cout << (path[i - 1] > 0 ? 'D' : 'U');
    } else {
      std::cout << (path[i - 1] > 0 ? 'R' : 'L');
    }
  }
}
