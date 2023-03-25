#include <iostream>
#include <vector>

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

void Merge(std::pair<int, int>* a, int left, int right, int mid) {
  int k = 0;
  std::pair<int, int>* temp = new std::pair<int, int>[right - left + 1];
  int i = left;
  int j = mid + 1;
  while (i <= mid and j <= right) {
    if (a[j].first == a[i].first) {
      if (a[j].second > a[i].second) {
        temp[k] = a[j];
        k++;
        j++;
      } else {
        temp[k] = a[i];
        k++;
        i++;
      }

    } else if (a[j] < a[i]) {
      temp[k] = a[j];
      k++;
      j++;
    } else {
      temp[k] = a[i];
      k++;
      i++;
    }
  }
  while (i <= mid) {
    temp[k] = a[i];
    k++;
    i++;
  }
  while (j <= right) {
    temp[k] = a[j];
    k++;
    j++;
  }
  for (int v = 0; v < right - left + 1; v++) {
    a[left + v] = temp[v];
  }
  delete[] temp;
}

void MergeSort(std::pair<int, int>* a, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    MergeSort(a, left, mid);
    MergeSort(a, mid + 1, right);
    Merge(a, left, right, mid);
  }
}

class SegmentTree {
 public:
  SegmentTree(int n) : size_(Clp2(n)), arr_(new int[2 * size_]) {
    for (int i = 0; i < size_; ++i) {
      arr_[i + size_] = 0;
    }
    for (int i = size_ - 1; i > 0; --i) {
      arr_[i] = std::max(arr_[i << 1], arr_[i << 1 | 1]);
    }
  }

  ~SegmentTree() { delete[] arr_; }

  int Max(int left, int right) {
    int max = 0;
    left += size_;
    right += size_;
    for (; left < right; left >>= 1, right >>= 1) {
      if ((left & 1) != 0) {
        max = std::max(max, arr_[left++]);
      }
      if ((right & 1) != 0) {
        max = std::max(max, arr_[--right]);
      }
    }
    return max;
  }
  void Insert(int pos, int value) {
    pos = pos + size_;
    arr_[pos] = value;
    for (int i = pos; i > 1; i >>= 1) {
      arr_[i >> 1] = std::max(arr_[i], arr_[i ^ 1]);
    }
  }

 private:
  int size_;
  int* arr_;
};

int Lcs(std::pair<int, int>* arr, int n) {
  MergeSort(arr, 0, n - 1);
  SegmentTree* tree = new SegmentTree(n);
  for (int i = 0; i < n; ++i) {
    int max = tree->Max(0, arr[i].second);
    tree->Insert(arr[i].second, max + 1);
  }
  int answ = tree->Max(0, n);
  delete tree;
  return answ;
}

int main() {
  int n;
  std::cin >> n;
  std::pair<int, int>* arr = new std::pair<int, int>[n];
  for (int i = 0; i < n; ++i) {
    int temp;
    std::cin >> temp;
    arr[i] = {temp, i};
  }
  std::cout << Lcs(arr, n);
  delete[] arr;
}
