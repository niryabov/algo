#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

enum Colors { White, Grey, Black };

void Time(size_t from,
          std::unordered_map<size_t, std::vector<size_t>>& edges_list,
          std::unordered_map<size_t, Colors>& colors, std::vector<int>& t_out) {
  colors[from] = Grey;
  std::vector<size_t>& nbors = edges_list[from];
  for (auto& nbor : nbors) {
    if (colors[nbor] == White) {
      Time(nbor, edges_list, colors, t_out);
    } else if (colors[nbor] == Grey) {
      t_out.push_back(-1);
      return;
    }
  }
  t_out.push_back(from);
  colors[from] = Black;
}

int main() {
  size_t vertex_num;
  size_t edge_num;
  std::cin >> vertex_num >> edge_num;
  std::unordered_map<size_t, Colors> colors;
  std::unordered_map<size_t, std::vector<size_t>> edges_list;

  for (size_t i = 0; i < edge_num; ++i) {
    size_t a;
    size_t b;
    std::cin >> a >> b;
    colors[a] = colors[b] = White;
    edges_list[a].push_back(b);
  }
  std::vector<int> t_out;
  Time(1, edges_list, colors, t_out);
  size_t ver = 2;
  while (t_out.size() != vertex_num) {
    if (colors[ver] == White) {
      Time(ver, edges_list, colors, t_out);
    }
    ++ver;
  }
  if (std::find(t_out.begin(), t_out.end(), -1) != t_out.end()) {
    std::cout << -1;
  } else {
    for (size_t i = t_out.size(); i > 0; --i) {
      std::cout << t_out[i - 1] << ' ';
    }
  }
}
