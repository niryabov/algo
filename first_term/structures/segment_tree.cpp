#include <iostream>

int Clp2(int x) {
  int p2 = 1;
  while (true) {
    if (p2 >= x) {
      return p2;
    }
    p2 <<= 1;
  }
  return 0;
}

class SegmentTree {
 public:
  SegmentTree(int n) : size_(Clp2(n)), arr_(new int[2 * size_]) {
    for (int i = 0; i < size_; ++i) {
      arr_[i + size_] = 0;
    }
    for (int i = size_ - 1; i > 0; --i) {
      arr_[i] = arr_[i << 1] + arr_[i << 1 | 1];
    }
  }

  ~SegmentTree() { delete[] arr_; }

  int Sum(int left, int right) {
    int sum = 0;
    left += size_;
    right += size_;
    for (; left < right; left >>= 1, right >>= 1) {
      if ((left & 1) != 0) {
        sum += arr_[left++];
      }
      if ((right & 1) != 0) {
        sum += arr_[--right];
      }
    }
    return sum;
  }
  void Insert(int pos, int value) {
    pos = pos + size_;
    arr_[pos] = value;
    for (int i = pos; i > 1; i >>= 1) {
      arr_[i >> 1] = arr_[i] + arr_[i ^ 1];
    }
  }

 private:
  int size_;
  int* arr_;
};

int main() {
  int n;
  std::cin >> n;
  SegmentTree* even = new SegmentTree(n);
  SegmentTree* odd = new SegmentTree(n);
  for (int i = 0; i < n; ++i) {
    int value;
    std::cin >> value;
    if (i % 2 == 0) {
      even->Insert(i, value);
    } else {
      odd->Insert(i, value);
    }
  }
  int reqs;
  std::cin >> reqs;
  for (int i = 0; i < reqs; ++i) {
    int op;
    int l;
    int r;
    std::cin >> op >> l >> r;
    if (op == 0) {
      if ((l - 1) % 2 == 0) {
        even->Insert(l - 1, r);
      } else {
        odd->Insert(l - 1, r);
      }
    } else {
      if ((l - 1) % 2 == 0) {
        std::cout << even->Sum(l - 1, r) - odd->Sum(l - 1, r) << '\n';
      } else {
        std::cout << odd->Sum(l - 1, r) - even->Sum(l - 1, r) << '\n';
      }
    }
  }
  delete odd;
  delete even;
}
