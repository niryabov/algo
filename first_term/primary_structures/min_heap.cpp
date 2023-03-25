#include <iostream>
#include <vector>

class MinHeap {
 public:
  MinHeap() : data_(kCapacity), size_(0) {}
  int Top() {
    if (size_ == 0) {
      return -1;
    }
    return data_[0];
  }
  int ExtractMin() {
    if (size_ == 0) {
      return -1;
    }
    std::swap(data_[0], data_[size_ - 1]);
    --size_;
    SiftDown(0);
    return data_[size_];
  }
  void Insert(int value) {
    if (size_ == data_.size()) {
      data_.resize(2 * size_);
    }
    data_[size_++] = value;
    SiftUp(size_ - 1);
  }
  bool Empty() { return size_ == 0; }

 private:
  void SiftUp(size_t index) {
    if (index > 0) {
      size_t parent = (index - 1) / 2;
      if (data_[index] < data_[parent]) {
        std::swap(data_[index], data_[parent]);
        SiftUp(parent);
      }
    }
  }
  void SiftDown(size_t index) {
    size_t i_min = index;
    size_t left_child = index * 2 + 1;
    size_t right_child = index * 2 + 2;
    if (left_child < size_ && data_[left_child] < data_[i_min]) {
      i_min = left_child;
    }
    if (right_child < size_ && data_[right_child] < data_[i_min]) {
      i_min = right_child;
    }
    if (index != i_min) {
      std::swap(data_[i_min], data_[index]);
      SiftDown(i_min);
    }
  }
  static constexpr size_t kCapacity = 8;
  std::vector<int> data_;
  size_t size_;
};

int main() {
  MinHeap* starts = new MinHeap;
  MinHeap* finishes = new MinHeap;
  int n;
  std::cin >> n;
  for (int i = 0; i < n; i++) {
    int left;
    int right;
    std::cin >> left >> right;
    starts->Insert(left);
    finishes->Insert(right);
  }
  std::vector<std::pair<int, int>>* segments =
      new std::vector<std::pair<int, int>>;
  while (!starts->Empty() && !finishes->Empty()) {
    int begin = starts->ExtractMin();
    int end = finishes->ExtractMin();
    while (!starts->Empty() && !finishes->Empty() && starts->Top() <= end) {
      starts->ExtractMin();
      end = finishes->ExtractMin();
    }
    segments->push_back({begin, end});
  }
  std::cout << segments->size() << '\n';
  for (auto i : *segments) {
    std::cout << i.first << ' ' << i.second << '\n';
  }
  delete starts;
  delete finishes;
  delete segments;
}
