#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <vector>

template <>
struct std::hash<std::pair<size_t, size_t>> {
  std::size_t operator()(std::pair<size_t, size_t> const& a) const noexcept {
    auto hasher = std::hash<size_t>{};
    size_t hash = hasher(a.first) + hasher(a.second);
    return hash;
  }
};

template <typename T, typename VertexType>
class AbstractMetric {
 public:
  virtual T operator()(VertexType from, VertexType to) noexcept = 0;
  virtual ~AbstractMetric() = default;
};

template <typename T, typename VertexType>
class Metric : public AbstractMetric<T, VertexType> {
 public:
  Metric() {}

  void SetDist(VertexType from, VertexType to, T dist) {
    dist_[{std::min(from, to), std::max(from, to)}] = dist;
  }

  T operator()(VertexType from, VertexType to) noexcept override {
    return dist_[{std::min(from, to), std::max(from, to)}];
  }

 private:
  std::unordered_map<std::pair<VertexType, VertexType>, T> dist_;
};

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
  using VertexType = VType;
  using EdgeType = EType;

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

template <typename VType = size_t, typename EType = std::pair<VType, VType>,
          typename MetricType = size_t>
class AdjacencyListGraph : public AbstractGraph<VType, EType> {
 public:
  AdjacencyListGraph(size_t vertices_num,
                     const std::vector<std::pair<EType, MetricType>>& edges)
      : AbstractGraph<VType, EType>(vertices_num, edges.size()) {
    for (const auto& edge : edges) {
      list_[edge.first.first].push_back(edge.first.second);
      list_[edge.first.second].push_back(edge.first.first);
      metric_.SetDist(edge.first.first, edge.first.second, edge.second);
    }
  }

  MetricType GetDist(VType from, VType to) { return metric_(from, to); }

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
  Metric<MetricType, VType> metric_;
};

template <class VType = size_t, class EType = std::pair<VType, VType>>
class Visitor {
 public:
  virtual void TreeEdge(const EType& /*edge*/) = 0;
  virtual ~Visitor() = default;
};

template <class VType = size_t, class EType = std::pair<VType, VType>,
          class MetricType = size_t>
class AncestorVisitor : Visitor<VType, EType> {
 public:
  void TreeEdge(const EType& /*edge*/){};
  void TreeEdge(const EType& edge, MetricType dist) {
    ancestors_[edge.second] = edge.first;
    distances_[edge.second] = dist;
  }

  std::unordered_map<VType, VType> GetAncestors() const { return ancestors_; }
  std::unordered_map<VType, MetricType> GetDistances() const {
    return distances_;
  }

  ~AncestorVisitor() = default;

 private:
  std::unordered_map<VType, VType> ancestors_;
  std::unordered_map<VType, MetricType> distances_;
};

template <class VType = size_t, class Graph = AdjacencyListGraph<>,
          class MetricType = size_t, class Visitor = AncestorVisitor<>>
void Dijkstra(VType from, Graph& graph, Visitor& visitor) {
  std::vector<MetricType> distances(graph.GetVerticesNumber(), 2009000999);
  distances[from] = MetricType();
  std::priority_queue<std::pair<MetricType, VType>,
                      std::vector<std::pair<MetricType, VType>>,
                      std::greater<std::pair<MetricType, VType>>>
      queue;
  queue.emplace(distances[from], from);
  while (!queue.empty()) {
    auto[dist, now_near] = queue.top();
    queue.pop();
    if (dist > distances[now_near]) {
      continue;
    }
    for (auto& nbor : graph.GetNeighbours(now_near)) {
      MetricType distance = graph.GetDist(now_near, nbor);
      if (distances[now_near] + distance < distances[nbor]) {
        distances[nbor] = distances[now_near] + distance;
        visitor.TreeEdge({now_near, nbor}, distances[nbor]);
        queue.emplace(distances[nbor], nbor);
      }
    }
  }
  for (size_t i = 0; i < graph.GetVerticesNumber(); ++i) {
    if (distances[i] == 2009000999) {
      visitor.TreeEdge({i, i}, distances[i]);
    }
  }
}

void Solve() {
  size_t ver_num, edge_num;
  std::cin >> ver_num >> edge_num;
  std::vector<std::pair<std::pair<size_t, size_t>, size_t>> edges;
  edges.reserve(edge_num);
  for (size_t i = 0; i < edge_num; ++i) {
    size_t from, to, weight;
    std::cin >> from >> to >> weight;
    edges.push_back({{from, to}, weight});
  }
  size_t from;
  std::cin >> from;
  AdjacencyListGraph<size_t, std::pair<size_t, size_t>, size_t> graph(ver_num,
                                                                      edges);
  AncestorVisitor<size_t, std::pair<size_t, size_t>, size_t> visitor;
  Dijkstra(from, graph, visitor);
  std::unordered_map<size_t, size_t> dists = visitor.GetDistances();
  for (size_t i = 0; i < ver_num; ++i) {
    std::cout << dists[i] << ' ';
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t n;
  std::cin >> n;
  for (size_t i = 0; i < n; ++i) {
    Solve();
    if (i != n - 1) {
      std::cout << '\n';
    }
  }
}
