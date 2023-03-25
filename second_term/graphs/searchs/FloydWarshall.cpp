#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <unordered_map>
#include <vector>
template <>
struct std::hash<std::pair<int64_t, int64_t>> {
  std::size_t operator()(std::pair<int64_t, int64_t> const& a) const noexcept {
    auto hasher = std::hash<int64_t>{};
    int64_t hash = hasher(a.first) + hasher(a.second);
    return hash;
  }
};
static const int64_t kInfty = std::numeric_limits<int64_t>::max();

bool MakePath(std::vector<int64_t>& cities,
              std::vector<std::vector<int64_t>>& parents,
              std::unordered_map<std::pair<int64_t, int64_t>, int64_t>& races,
              std::vector<std::vector<int64_t>>& graph) {
  std::vector<int> path;
  for (size_t i = 0; i < cities.size() - 1; ++i) {
    int64_t from = cities[i];
    int64_t to = cities[i + 1];
    int64_t temp = from;
    for (size_t i = 0; i < graph.size(); ++i) {
      if (graph[i][i] < 0 && graph[from][i] != kInfty &&
          graph[i][to] != kInfty) {
        return true;
      }
    }

    while (temp != to) {
      path.push_back(races[{temp, parents[temp][to]}]);
      temp = parents[temp][to];
    }
  }
  std::cout << path.size() << '\n';
  for (auto i : path) {
    std::cout << i << ' ';
  }

  return false;
}

bool FordWarshall(std::vector<std::vector<int64_t>>& graph,
                  std::vector<std::vector<int64_t>>& parents,
                  std::vector<int64_t>& cities) {
  int64_t vertex_num = graph.size();

  for (int64_t k = 0; k < vertex_num; ++k) {
    for (int64_t i = 0; i < vertex_num; ++i) {
      for (int64_t j = 0; j < vertex_num; j++) {
        if (graph[i][k] + graph[k][j] < graph[i][j] && graph[i][k] != kInfty &&
            graph[k][j] != kInfty) {
          graph[i][j] = graph[i][k] + graph[k][j];
          parents[i][j] = parents[i][k];
        }
      }
    }
  }

  for (auto i : cities) {
    if (graph[i][i] < 0) {
      return true;
    }
  }
  return false;
}

int main() {
  int64_t vertex_num;
  int64_t edges_num;
  int64_t cities_num;
  std::cin >> vertex_num >> edges_num >> cities_num;
  std::vector<std::vector<int64_t>> graph(
      vertex_num, std::vector<int64_t>(vertex_num, kInfty));
  std::vector<std::vector<int64_t>> parents(
      vertex_num, std::vector<int64_t>(vertex_num, kInfty));
  std::unordered_map<std::pair<int64_t, int64_t>, int64_t> races;
  for (int64_t i = 0; i < edges_num; ++i) {
    int64_t from, to, weight;
    std::cin >> from >> to >> weight;
    graph[from - 1][to - 1] = -1 * weight;
    parents[from - 1][to - 1] = to - 1;
    races[{from - 1, to - 1}] = i + 1;
  }
  std::vector<int64_t> cities;
  cities.reserve(cities_num);
  for (int64_t i = 0; i < cities_num; ++i) {
    int64_t temp;
    std::cin >> temp;
    cities.push_back(temp - 1);
  }

  if (FordWarshall(graph, parents, cities)) {
    std::cout << "infinitely kind";
    return 0;
  }
  if (MakePath(cities, parents, races, graph)) {
    std::cout << "infinitely kind";
  }
}
