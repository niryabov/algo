#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <set>
#include <unordered_map>
#include <vector>

static const size_t kInfty = std::numeric_limits<size_t>::max();

class SparseTable {
 public:
  SparseTable(const std::vector<size_t>& data) {
    logs_ = std::vector<size_t>(data.size() + 1, 0);
    n_ = data.size();

    PrecalcLogs(data.size() + 2);
    log_n_ = logs_[n_];
    // sparse_.assign(log_n_ + 1, std::vector<size_t>(n_ + 1, size_t()));
    sparse_ = std::vector<std::vector<size_t>>(log_n_ + 1,
                                               std::vector<size_t>(n_ + 1, 0));
    BuildSparse(data);
  }

  size_t Query(size_t l, size_t r) {  // interval [l, r)
    size_t lg = logs_[r - l];
    return std::min(sparse_[lg][l], sparse_[lg][r - (1 << lg)]);
  }

 private:
  void BuildSparse(const std::vector<size_t>& data) {
    sparse_[0] = data;
    for (size_t k = 1; k <= log_n_; ++k) {
      for (size_t i = 0; i + (1 << k) <= n_; ++i) {
        sparse_[k][i] =
            std::min(sparse_[k - 1][i], sparse_[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  void PrecalcLogs(size_t n) {
    logs_[0] = 0;
    logs_[1] = 0;
    for (size_t i = 2; i < n - 1; ++i) {
      logs_[i] = logs_[i / 2] + 1;
    }
  }

  size_t n_;
  size_t log_n_;
  std::vector<size_t> logs_;
  std::vector<std::vector<size_t>> sparse_;
};

template <typename size_t>
class MinFunctor {
 public:
  size_t operator()(const size_t& a, const size_t& b) { return a < b ? a : b; }
};
