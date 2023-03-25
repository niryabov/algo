#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

static constexpr size_t kInfty = std::numeric_limits<size_t>::max();

struct Input {
  Input(const std::vector<std::vector<size_t>>& graph,
        const std::vector<std::pair<size_t, size_t>>& edges,
        std::vector<size_t>& time_in, std::vector<size_t>& f_up)
      : graph(graph), edges(edges), time_in(time_in), f_up(f_up) {}
  const std::vector<std::vector<size_t>>& graph;
  const std::vector<std::pair<size_t, size_t>>& edges;
  std::vector<size_t>& time_in;
  std::vector<size_t>& f_up;
};

void BridgesDfs(Input& a,
                size_t vertex,   // vertex now
                size_t edge_id,  // edge to vertex
                size_t& time, std::vector<size_t>& bridges) {
  const std::vector<std::vector<size_t>>& graph = a.graph;
  const std::vector<std::pair<size_t, size_t>>& edges = a.edges;
  std::vector<size_t>& time_in = a.time_in;
  std::vector<size_t>& f_up = a.f_up;
  time_in[vertex] = time++;
  f_up[vertex] = time_in[vertex];
  for (auto from_edge_id : graph[vertex]) {
    if (from_edge_id == edge_id) {
      continue;
    }
    auto& edge = edges[from_edge_id];
    size_t to = edge.first == vertex ? edge.second : edge.first;
    if (to == vertex) {  // loop
      continue;
    }
    if (time_in[to] != kInfty) {
      f_up[vertex] = std::min(f_up[vertex], time_in[to]);
    } else {
      BridgesDfs(a, to, from_edge_id, time, bridges);
      f_up[vertex] = std::min(f_up[to], f_up[vertex]);
    }
    if (f_up[to] > time_in[vertex]) {
      bridges.push_back(from_edge_id);
    }
  }
}

std::vector<size_t> GetBridges(
    const std::vector<std::vector<size_t>>& graph,
    const std::vector<std::pair<size_t, size_t>>& edges) {
  size_t num_vertex = graph.size();
  std::vector<size_t> time_in(num_vertex, kInfty);  // time in
  // std::vector<size_t> time_out(num_vertex, kInfty);
  std::vector<size_t> f_up(num_vertex,
                           kInfty);  // aka RET(from lecture) magic function :)
  std::vector<size_t> bridges;       // result - all bridges
  size_t time = 0;                   // time now
  Input a(graph, edges, time_in, f_up);
  for (size_t i = 0; i < num_vertex; ++i) {
    if (time_in[i] == kInfty) {
      BridgesDfs(a, i, kInfty, time, bridges);
    }
  }
  return bridges;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  std::vector<std::vector<size_t>> graph(
      n + 1, std::vector<size_t>());             // edges from v (id)
  std::vector<std::pair<size_t, size_t>> edges;  // all edges

  edges.reserve(m);
  for (size_t id = 0; id < m; ++id) {
    size_t from;
    size_t to;
    std::cin >> from >> to;
    edges.emplace_back(std::min(from, to),
                       std::max(from, to));  // make first <= second
    // unoriented edge
    graph[from].push_back(id);
    graph[to].push_back(id);
  }
  // return all bridges id
  auto bridges = GetBridges(graph, edges);
  std::cout << bridges.size() << '\n';
  std::sort(bridges.begin(), bridges.end());
  for (auto id : bridges) {
    std::cout << id + 1 << ' ';
  }

  return 0;
}
