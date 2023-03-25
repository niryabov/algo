#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::vector<std::queue<size_t>> at_dist(31);
  size_t vertexes;
  size_t edges;
  std::cin >> vertexes >> edges;

  std::vector<bool> used(vertexes, false);
  size_t start;
  size_t finish;
  std::cin >> start >> finish;
  std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> edges_list;
  for (size_t i = 0; i < edges; ++i) {
    size_t a;
    size_t b;
    size_t w;
    std::cin >> a >> b >> w;
    if (edges_list.find(a) != edges_list.end()) {
      edges_list[a].push_back({b, w});
    } else {
      edges_list[a] = std::vector<std::pair<size_t, size_t>>();
      edges_list[a].push_back({b, w});
    }
  }

  size_t cur_dist = 0;
  size_t to_visit = 1;
  at_dist[0].push(start);
  while (to_visit > 0) {
    while (at_dist[cur_dist % 31].empty()) {
      ++cur_dist;
    }

    while (!at_dist[cur_dist % 31].empty()) {
      size_t vertex = at_dist[cur_dist % 31].front();
      if (vertex == finish) {
        std::cout << cur_dist;
        return 0;
      }

      if (!used[vertex]) {
        std::vector<std::pair<size_t, size_t>>& nbours = edges_list[vertex];
        for (auto& nbour : nbours) {
          if (!used[nbour.first]) {
            at_dist[(cur_dist + nbour.second) % 31].push(nbour.first);
            ++to_visit;
          }
        }
        used[vertex] = true;
      }
      at_dist[cur_dist % 31].pop();
      --to_visit;
    }
  }
  std::cout << -1;
}
