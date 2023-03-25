#include <iostream>

class AVLTree {
  struct Node {
    Node* left{nullptr};
    Node* right{nullptr};
    int64_t sum{0};
    int value;
    unsigned char height{0};
  };

 public:
  void Insert(int value) { root_ = Insert(root_, value); }

  bool Find(int value) { return Find(root_, value); }

  void Erase(int value) { root_ = Erase(root_, value); }

  int64_t Sum(int l, int r) { return Sum(root_, l, r); }

  int64_t RangeSum(int l, int r) { return RangeSum(root_, l, r, -1); }
  ~AVLTree() { Clear(root_); }

 private:
  static Node* Erase(Node* node, int value) {
    if (node == nullptr) {
      return nullptr;
    }
    if (node->value == value) {
      if (node->right == nullptr) {
        Node* temp = node->left;
        delete node;
        return FixBalance(temp);
      }
      Node* min_right = FindMin(node->right);
      node->right = UnlinkMin(node->right);
      min_right->left = node->left;
      min_right->right = node->right;
      delete node;
      return FixBalance(min_right);
    }
    if (value <= node->value) {
      node->left = Erase(node->left, value);
    } else {
      node->right = Erase(node->right, value);
    }
    return FixBalance(node);
  }

  static Node* FindMin(Node* node) {
    if (node->left == nullptr) {
      return node;
    }
    return FindMin(node->left);
  }
  static int64_t Sum(Node* node, int left_border, int right_border) {
    if (node == nullptr) {
      return 0;
    }
    int64_t sum = 0;
    if (left_border <= node->value and node->value <= right_border) {
      sum += node->value;
    }
    if (node->value > left_border) {
      sum += Sum(node->left, left_border, right_border);
    }
    if (node->value < right_border) {
      sum += Sum(node->right, left_border, right_border);
    }
    return sum;
  }
  static Node* UnlinkMin(Node* node) {
    if (node->left == nullptr) {
      return node->right;
    }
    node->left = UnlinkMin(node->left);
    return FixBalance(node);
  }

  static bool Find(Node* node, int value) {
    if (node == nullptr) {
      return false;
    }
    if (node->value == value) {
      return true;
    }
    if (value <= node->value) {
      return Find(node->left, value);
    }
    return Find(node->right, value);
  }

  static Node* Insert(Node* node, int value) {
    if (node == nullptr) {
      Node* new_node = new Node;
      new_node->value = value;
      new_node->sum = value;
      return new_node;
    }
    if (value < node->value) {
      node->left = Insert(node->left, value);
      FixSum(node->left);
      FixSum(node);
    }
    if (value > node->value) {
      node->right = Insert(node->right, value);
      FixSum(node->right);
      FixSum(node);
    }
    return FixBalance(node);
  }

  static void FixHeight(Node* node) {
    if (node == nullptr) {
      return;
    }
    node->height = std::max(Height(node->left), Height(node->right)) + 1;
  }

  static void FixSum(Node* node) {
    if (node == nullptr) {
      return;
    }
    node->sum = GetSum(node->left) + GetSum(node->right) + node->value;
  }
  static int64_t GetSum(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->sum;
  }

  int64_t RangeSum(Node* node, int left_border, int right_border,
                   int previousvalue) {
    int64_t sum = 0;
    if (node == nullptr) {
      return 0;
    }
    if (left_border <= node->value and node->value <= right_border) {
      sum += node->value;
    }
    if (previousvalue > node->value and previousvalue <= right_border and
        node->value >= left_border) {
      sum += GetSum(node->right);
      sum += RangeSum(node->left, left_border, right_border, node->value);
      return sum;
    }

    if (previousvalue < node->value and previousvalue >= left_border and
        node->value <= right_border) {
      sum += GetSum(node->left);
      sum += RangeSum(node->right, left_border, right_border, node->value);
      return sum;
    }

    if (node->value >= left_border) {
      sum += RangeSum(node->left, left_border, right_border, node->value);
    }
    if (node->value <= right_border) {
      sum += RangeSum(node->right, left_border, right_border, node->value);
    }
    return sum;
  }

  static Node* RotateRight(Node* p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    FixSum(p);
    FixSum(q);
    FixHeight(p);
    FixHeight(q);
    return q;
  }

  static Node* RotateLeft(Node* q) {
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    FixSum(q);
    FixSum(p);
    FixHeight(q);
    FixHeight(p);
    return p;
  }

  static Node* FixBalance(Node* node) {
    FixHeight(node);
    if (BalanceFactor(node) == 2) {
      if (BalanceFactor(node->right) == -1) {
        node->right = RotateRight(node->right);
        FixSum(node);
      }
      return RotateLeft(node);
    }
    if (BalanceFactor(node) == -2) {
      if (BalanceFactor(node->left) == 1) {
        node->left = RotateLeft(node->left);
        FixSum(node);
      }
      return RotateRight(node);
    }
    return node;
  }

  static int Height(Node* node) { return node == nullptr ? 0 : node->height; }

  static int BalanceFactor(Node* node) {
    return node == nullptr ? 0 : Height(node->right) - Height(node->left);
  }
  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->right);
    Clear(node->left);
    delete node;
  }

  Node* root_{nullptr};
};

int main() {
  // std::ios_base::sync_with_stdio(false);
  // std::cin.tie(nullptr);
  // std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  int64_t lastans = 0;
  char op;
  int value;
  int value1;
  AVLTree* tree = new AVLTree;
  for (int i = 0; i < n; i++) {
    std::cin >> op;
    if (op == '+') {
      std::cin >> value;
      tree->Insert((value + lastans) % 1000000000);
      lastans = 0;
    }
    if (op == '?') {
      std::cin >> value >> value1;
      lastans = tree->RangeSum(value, value1);
      std::cout << lastans << '\n';
    }
  }

  delete tree;
}
