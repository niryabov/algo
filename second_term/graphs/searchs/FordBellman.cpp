
#include <iostream>
#include <limits>
#include <vector>

static const int64_t kInfty = std::numeric_limits<int64_t>::max();
static const int64_t kNull = 100000;

bool FordBellman(int vertex, std::vector<std::vector<int64_t>>& graph,
                 std::vector<bool>& visited, int64_t vertex_num) {
  std::vector<int64_t> dp(vertex_num, kInfty);
  std::vector<int64_t> parents(vertex_num, 0);
  dp[vertex] = 0;

  for (int64_t k = 1; k < vertex_num; ++k) {
    for (auto& edge : graph) {
      if (dp[edge[0]] + edge[2] < dp[edge[1]] && dp[edge[0]] != kInfty) {
        visited[edge[0]] = true;
        visited[edge[1]] = true;
        dp[edge[1]] = dp[edge[0]] + edge[2];
        parents[edge[1]] = edge[0];
      }
    }
  }
  int64_t answ = kInfty;
  for (auto& edge : graph) {
    if (dp[edge[0]] + edge[2] < dp[edge[1]] && dp[edge[0]] != kInfty) {
      answ = edge[1];
    }
  }
  if (answ == kInfty) {
    // std::cout << "NO";
    return false;
  }
  std::vector<int64_t> path;

  int temp = answ;

  std::vector<bool> used(vertex_num, false);

  while (!used[temp]) {
    used[temp] = true;
    temp = parents[temp];
  }

  answ = temp;
  path.push_back(answ + 1);
  temp = parents[answ];

  while (temp != answ) {
    path.push_back(temp + 1);
    temp = parents[temp];
  }
  path.push_back(answ + 1);

  std::cout << "YES\n" << path.size() << '\n';
  for (size_t i = path.size(); i > 0; --i) {
    std::cout << path[i - 1] << ' ';
  }
  return true;
}

void Solve(std::vector<std::vector<int64_t>>& graph, int vertex_num) {
  std::vector<bool> used(vertex_num, false);
  bool answ = FordBellman(0, graph, used, vertex_num);
  if (answ) {
    return;
  }
  for (int i = 1; i < vertex_num; ++i) {
    if (!used[i]) {
      answ = FordBellman(i, graph, used, vertex_num);
      if (answ) {
        return;
      }
    }
  }
  std::cout << "NO";
}

int main() {
  int64_t n;
  std::cin >> n;
  std::vector<std::vector<int64_t>> graph;
  for (int64_t i = 0; i < n; ++i) {
    for (int64_t j = 0; j < n; ++j) {
      int64_t temp;
      std::cin >> temp;
      if (temp != kNull) {
        graph.push_back({i, j, temp});
      }
    }
  }
  Solve(graph, n);
}
