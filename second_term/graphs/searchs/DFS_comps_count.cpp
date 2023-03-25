#include <iostream>
#include <unordered_map>
#include <vector>

enum Colors { White, Grey, Black };

struct Graph {
  Graph(std::unordered_map<size_t, Colors>& colors,
        std::unordered_map<size_t, std::vector<size_t>>& edges_list)
      : grapgh_colors(colors), graph_edge(edges_list) {}
  std::unordered_map<size_t, Colors>& grapgh_colors;
  std::unordered_map<size_t, std::vector<size_t>>& graph_edge;
};

void DFS(size_t origin, Graph& graph,
         std::vector<std::vector<size_t>>& components, int& component,
         size_t& used) {
  std::unordered_map<size_t, std::vector<size_t>>& edges_list =
      graph.graph_edge;
  std::unordered_map<size_t, Colors>& colors = graph.grapgh_colors;
  colors[origin] = Grey;
  ++used;
  components[component].push_back(origin);
  std::vector<size_t>& nbors = edges_list[origin];
  for (auto& nbor : nbors) {
    if (colors[nbor] == White) {
      DFS(nbor, graph, components, component, used);
    }
  }
  colors[origin] = Black;
}

void Solve(size_t vert_num,
           std::unordered_map<size_t, std::vector<size_t>>& edges_list,
           std::vector<std::vector<size_t>>& components,
           std::unordered_map<size_t, Colors>& colors) {
  size_t now_vert = 1;
  int component = 0;
  size_t used = 0;
  Graph graph(colors, edges_list);

  while (used != vert_num) {
    if (colors[now_vert] == White) {
      components.push_back({});

      DFS(now_vert, graph, components, component, used);
      ++component;
    }
    ++now_vert;
  }
}

int main() {
  size_t vert_num;
  size_t edges_num;
  std::cin >> vert_num >> edges_num;
  std::unordered_map<size_t, std::vector<size_t>> edges_list;
  std::unordered_map<size_t, Colors> colors;

  for (size_t i = 0; i < edges_num; ++i) {
    size_t a;
    size_t b;
    std::cin >> a >> b;
    edges_list[a].push_back(b);
    edges_list[b].push_back(a);
    colors[a] = colors[b] = White;
  }
  std::vector<std::vector<size_t>> components;
  Solve(vert_num, edges_list, components, colors);
  std::cout << components.size() << '\n';
  for (auto& component : components) {
    std::cout << component.size() << '\n';
    for (auto vertex : component) {
      std::cout << vertex << ' ';
    }
    std::cout << '\n';
  }
}
