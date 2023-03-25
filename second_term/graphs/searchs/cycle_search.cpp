#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

enum Colors { White, Grey, Black };

void Time(size_t from,
          std::unordered_map<size_t, std::vector<size_t>>& edges_list,
          std::unordered_map<size_t, Colors>& colors,
          std::vector<size_t>& t_out) {
  colors[from] = Grey;
  std::vector<size_t>& nbors = edges_list[from];
  for (auto& nbor : nbors) {
    if (colors[nbor] == White) {
      Time(nbor, edges_list, colors, t_out);
    }
  }
  t_out.push_back(from);
  colors[from] = Black;
}

void Show(size_t from,
          std::unordered_map<size_t, std::vector<size_t>>& edges_list,
          std::unordered_map<size_t, Colors>& colors) {
  colors[from] = Grey;

  std::vector<size_t>& nbors = edges_list[from];
  for (auto& nbor : nbors) {
    if (colors[nbor] == White) {
      Show(nbor, edges_list, colors);
    }
  }
  std::cout << from << ' ';
  colors[from] = Black;
}

bool IsCycle(size_t from,
             std::unordered_map<size_t, std::vector<size_t>>& edges_list,
             std::unordered_map<size_t, Colors>& colors) {
  colors[from] = Grey;

  for (size_t nbor_i = 0; nbor_i < edges_list[from].size(); ++nbor_i) {
    if (colors[edges_list[from][nbor_i]] == White) {
      std::cout << "YES" << '\n';
      for (size_t j = nbor_i + 1; j < edges_list[from].size(); ++j) {
        colors[edges_list[from][j]] = Black;
      }
      Show(from, edges_list, colors);
      return true;
    }
  }
  return false;
}

int main() {
  size_t vertex_num;
  size_t edge_num;
  std::cin >> vertex_num >> edge_num;
  std::unordered_map<size_t, Colors> colors;
  std::unordered_map<size_t, Colors> trans_colors;
  std::unordered_map<size_t, std::vector<size_t>> edges_list;
  std::unordered_map<size_t, std::vector<size_t>> trans_edges_list;

  for (size_t i = 0; i < edge_num; ++i) {
    size_t a;
    size_t b;
    std::cin >> a >> b;
    trans_colors[a] = trans_colors[b] = colors[a] = colors[b] = White;
    edges_list[a].push_back(b);
    trans_edges_list[b].push_back(a);
  }
  std::vector<size_t> t_out;
  std::vector<bool> used;
  Time(1, edges_list, colors, t_out);
  size_t ver = 2;
  while (t_out.size() != vertex_num) {
    if (colors[ver] == White) {
      Time(ver, edges_list, colors, t_out);
    }
    ++ver;
  }
  for (size_t i = vertex_num; i > 0; --i) {
    if (IsCycle(t_out[i - 1], trans_edges_list, trans_colors)) {
      return 0;
    }
  }
  std::cout << "NO";
}
