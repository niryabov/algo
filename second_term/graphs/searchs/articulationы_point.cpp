#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <unordered_map>
#include <vector>

static constexpr size_t kInfty = std::numeric_limits<size_t>::max();

struct Input {
  Input(std::unordered_map<size_t, std::vector<size_t>>& edges,
        std::vector<size_t>& time_in, std::vector<size_t>& f_up,
        std::set<size_t>& points)
      : edges(edges), time_in(time_in), f_up(f_up), points(points) {}
  std::unordered_map<size_t, std::vector<size_t>>& edges;
  std::vector<size_t>& time_in;
  std::vector<size_t>& f_up;
  std::set<size_t>& points;
};

void DFS(Input& a, size_t vertex, size_t& time, bool is_root) {
  std::unordered_map<size_t, std::vector<size_t>>& edges = a.edges;
  std::vector<size_t>& time_in = a.time_in;
  std::vector<size_t>& f_up = a.f_up;
  std::set<size_t>& points = a.points;

  time_in[vertex] = ++time;
  f_up[vertex] = time;
  size_t childs = 0;
  for (auto nbor : edges[vertex]) {
    if (nbor == vertex) {
      continue;
    }
    if (time_in[nbor] != kInfty) {
      f_up[vertex] = std::min(f_up[vertex], time_in[nbor]);
    } else {
      ++childs;
      DFS(a, nbor, time, false);
      f_up[vertex] = std::min(f_up[vertex], f_up[nbor]);
      if (!is_root && f_up[nbor] >= time_in[vertex]) {
        points.insert(vertex + 1);
      }
    }
  }
  if (is_root && childs > 1) {
    points.insert(vertex + 1);
  }
}

std::set<size_t> GetPoints(
    size_t vertex_num, std::unordered_map<size_t, std::vector<size_t>>& edges) {
  std::vector<size_t> time_in(vertex_num, kInfty);
  std::vector<size_t> f_up(vertex_num, kInfty);
  std::set<size_t> points;
  size_t time = 0;

  Input a(edges, time_in, f_up, points);

  for (size_t vertex = 0; vertex < vertex_num; ++vertex) {
    if (time_in[vertex] == kInfty) {
      DFS(a, vertex, time, true);
    }
  }

  return points;
}

int main() {
  size_t vertex_num;
  size_t edges_num;
  std::cin >> vertex_num >> edges_num;
  std::unordered_map<size_t, std::vector<size_t>> edges;
  for (size_t i = 0; i < edges_num; ++i) {
    size_t from;
    size_t to;
    std::cin >> from >> to;
    edges[from - 1].push_back(to - 1);
    edges[to - 1].push_back(from - 1);
  }
  std::set<size_t> points = GetPoints(vertex_num, edges);
  std::cout << points.size() << '\n';
  for (auto point : points) {
    std::cout << point << ' ';
  }
}
