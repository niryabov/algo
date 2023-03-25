#include <iostream>
#include <vector>

class Fenwick {
 public:
  Fenwick(int size)
      : size_(size),
        data_(std::vector(size, std::vector(size, std::vector(size, 0)))) {}

  int64_t Sum(int x1, int y1, int z1, int x2, int y2, int z2) {
    if (x1 == 0 && y1 == 0 && z1 == 0) {
      return Sum(x2, y2, z2);
    }
    return Sum(x2, y2, z2) - Sum(x2, y2, z1 - 1) - Sum(x1 - 1, y2, z2) -
           Sum(x2, y1 - 1, z2) + Sum(x1 - 1, y2, z1 - 1) +
           Sum(x2, y1 - 1, z1 - 1) + Sum(x1 - 1, y1 - 1, z2) -
           Sum(x1 - 1, y1 - 1, z1 - 1);
  }

  void Add(int x, int y, int z, int value) {
    for (int i = x; i < size_; i = (i | (i + 1))) {
      for (int j = y; j < size_; j = (j | (j + 1))) {
        for (int k = z; k < size_; k = (k | (k + 1))) {
          data_[i][j][k] += value;
        }
      }
    }
  }

 private:
  int64_t Sum(int x, int y, int z) {
    int64_t sum = 0;
    for (int i = x; i >= 0; i = ((i & (i + 1)) - 1)) {
      for (int j = y; j >= 0; j = ((j & (j + 1)) - 1)) {
        for (int k = z; k >= 0; k = ((k & (k + 1)) - 1)) {
          sum += data_[i][j][k];
        }
      }
    }
    return sum;
  }

  int size_;
  std::vector<std::vector<std::vector<int>>> data_;
};

bool Request(int command, Fenwick* tree) {
  if (command == 1) {
    int x;
    int y;
    int z;
    int value;
    std::cin >> x >> y >> z >> value;
    tree->Add(x, y, z, value);
    return true;
  }
  if (command == 2) {
    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    std::cout << tree->Sum(x1, y1, z1, x2, y2, z2) << '\n';
    return true;
  }
  return false;
}

void Command(Fenwick* tree) {
  bool input = true;
  int command;

  while (input) {
    std::cin >> command;
    input = Request(command, tree);
  }
}

int main() {
  int n;
  std::cin >> n;
  Fenwick* tree = new Fenwick(n);
  Command(tree);
  delete tree;
}
