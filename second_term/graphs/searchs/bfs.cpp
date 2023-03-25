
#include <functional>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <class T>
using FilterFunction = std::function<bool(const T&)>;

template <class VType = size_t, class EType = std::pair<VType, VType>>
class IteratorImpl {
 public:
  using Iterator = IteratorImpl;

  IteratorImpl(VType v, typename std::vector<VType>::iterator begin,
               typename std::vector<VType>::iterator end,
               const FilterFunction<EType>& filter)
      : v_(v), now_(begin), end_(end), filter_(filter) {
    if (now_ != end_) {
      now_edge_.first = v_;
      now_edge_.second = *now_;
      if (!filter_(now_edge_)) {
        ++*this;
      }
    }
  }
  IteratorImpl& operator++() {
    do {
      ++now_;
      if (now_ == end_) {
        return *this;
      }
      now_edge_ = {v_, *now_};
    } while (!filter_(now_edge_) && now_ != end_);
    return *this;
  }

  bool operator==(const IteratorImpl& other) const {
    return now_ == other.now_;
  }
  bool operator!=(const IteratorImpl& other) const {
    return now_ != other.now_;
  }

  IteratorImpl Begin() { return *this; }

  IteratorImpl End() { return {v_, end_, end_, filter_}; }

  const EType& operator*() { return now_edge_; }

 private:
  EType now_edge_;
  VType v_;
  typename std::vector<VType>::iterator now_;
  typename std::vector<VType>::iterator end_;
  const FilterFunction<EType>& filter_;
};

template <typename VType = size_t, typename EType = std::pair<VType, VType>>
class AbstractGraph {
 public:
  using VTypeType = VType;
  using ETypeType = EType;

  explicit AbstractGraph(size_t vertices_num, size_t edges_num = 0)
      : vertices_number_(vertices_num), edges_number_(edges_num) {}

  size_t GetVerticesNumber() const { return vertices_number_; }
  size_t GetETypesNumber() const { return edges_number_; }

  virtual typename std::vector<VType>::iterator NeighboursBegin(VType v) = 0;
  virtual typename std::vector<VType>::iterator NeighboursEnd(VType v) = 0;
  virtual IteratorImpl<VType, EType> NeighboursIt(
      VType v, const FilterFunction<EType>& filter) = 0;

  virtual std::vector<VType> GetNeighbours(const VType& vertex) = 0;

 protected:
  size_t vertices_number_ = 0;
  size_t edges_number_ = 0;
};

template <typename VType = size_t, typename EType = std::pair<VType, VType>>
class AdjacencyListGraph : public AbstractGraph<VType, EType> {
 public:
  AdjacencyListGraph(size_t vertices_num, const std::vector<EType>& edges)
      : AbstractGraph<VType, EType>(vertices_num, edges.size()) {
    for (const auto& edge : edges) {
      list_[edge.first].push_back(edge.second);
      list_[edge.second].push_back(edge.first);
    }
  }

  std::vector<VType> GetNeighbours(const VType& vertex) final {
    return list_[vertex];
  }
  typename std::vector<VType>::iterator NeighboursBegin(VType v) override {
    return list_[v].begin();
  }
  typename std::vector<VType>::iterator NeighboursEnd(VType v) override {
    return list_[v].end();
  }
  IteratorImpl<VType, EType> NeighboursIt(
      VType v, const FilterFunction<EType>& filter) override {
    return {v, NeighboursBegin(v), NeighboursEnd(v), filter};
  }

 private:
  std::unordered_map<VType, std::vector<VType>> list_;
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class BfsVisitor {
 public:
  virtual void TreeEType(const EType& /*edge*/) = 0;
  virtual ~BfsVisitor() = default;
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class AncestorBfsVisitor : BfsVisitor<VType, EType> {
 public:
  virtual void TreeEType(const EType& edge) {
    ancestors_[edge.second] = edge.first;
  }

  std::unordered_map<VType, VType> GetMap() const { return ancestors_; }

  virtual ~AncestorBfsVisitor() = default;

 private:
  std::unordered_map<VType, VType> ancestors_;
};

template <class VType, class Graph, class Visitor>
void BreadthFirstSearch(VType origin_vertex, VType aim_vertex, Graph& graph,
                        Visitor& visitor) {
  std::queue<VType> bfs_queue;
  std::unordered_set<VType> visited_vertices;

  bfs_queue.push(origin_vertex);
  visited_vertices.insert(origin_vertex);

  while (!bfs_queue.empty()) {
    auto cur_vertex = bfs_queue.front();
    bfs_queue.pop();
    for (auto& neighbour : graph.GetNeighbours(cur_vertex)) {
      if (neighbour == aim_vertex) {
        visitor.TreeEType({cur_vertex, neighbour});
        return;
      }
      if (visited_vertices.find(neighbour) == visited_vertices.end()) {
        visitor.TreeEType({cur_vertex, neighbour});
        bfs_queue.push(neighbour);
        visited_vertices.insert(neighbour);
      }
    }
  }
}

int main() {
  size_t vertex_num;
  size_t edge_num;
  std::cin >> vertex_num >> edge_num;
  size_t start;
  size_t finish;
  std::cin >> start >> finish;
  AncestorBfsVisitor<size_t, std::pair<size_t, size_t>> visitor;
  std::vector<std::pair<size_t, size_t>> edges;
  for (size_t i = 0; i < edge_num; ++i) {
    size_t a;
    size_t b;
    std::cin >> a >> b;
    edges.push_back({a, b});
  }

  AdjacencyListGraph<size_t, std::pair<size_t, size_t>> graph(vertex_num,
                                                              edges);
  BreadthFirstSearch(start, finish, graph, visitor);
  std::unordered_map<size_t, size_t> ancestors = visitor.GetMap();
  std::vector<size_t> path;
  if (ancestors.find(finish) == ancestors.end()) {
    std::cout << -1;
    return 0;
  }

  size_t ancestor = finish;

  while (ancestor != start) {
    path.push_back(ancestor);
    ancestor = ancestors[ancestor];
  }
  path.push_back(start);

  std::cout << path.size() - 1 << '\n';
  for (size_t i = path.size(); i > 0; --i) {
    std::cout << path[i - 1] << ' ';
  }
}
