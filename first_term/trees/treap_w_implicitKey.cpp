#include <cstdlib>
#include <iostream>
#include <random>
#include <utility>

class Treap {
  struct Node {
    Node(int64_t priority, int value)
        : size(1),
          priority(priority),
          value(value),
          min(value),
          left(nullptr),
          right(nullptr) {}

    int64_t size;
    int64_t priority;
    int value;
    int min;
    Node* left;
    Node* right;
  };

 public:
  Treap() : root_(nullptr) {}
  ~Treap() { Clear(root_); }
  int64_t Size() { return Size(root_); }
  bool Empty() { return Size(root_) == 0; }
  void Erase(int64_t pos) {
    std::pair<Node*, Node*> temp = Split(root_, pos);
    Node* left = temp.first;
    Node* right_with_pos = temp.second;
    temp = Split(right_with_pos, 1);
    Node* pos_tree = temp.first;
    Node* right = temp.second;
    delete pos_tree;
    root_ = Merge(left, right);
  }

  void Insert(int64_t pos, int value) {
    int64_t priority = distribution_(gen_);
    Node* node = new Node(priority, value);
    std::pair<Node*, Node*> temp = Split(root_, pos);
    Node* first = temp.first;
    Node* second = temp.second;
    root_ = Merge(Merge(first, node), second);
  }

  int& operator[](int64_t pos) {
    std::pair<Node*, Node*> temp = Find(nullptr, root_, pos);
    Node* pos_node = temp.second;
    return pos_node->value;
  }
  int Min(int left, int right) {
    std::pair<Node*, Node*> temp = Split(root_, left);
    Node* first = temp.first;
    Node* second = temp.second;
    std::pair<Node*, Node*> temp2 = Split(second, right - left + 1);
    int min = GetMin(temp2.first);
    root_ = Merge(first, Merge(temp2.first, temp2.second));
    return min;
  }

 private:
  std::pair<Node*, Node*> Find(Node* parent, Node* node, int64_t pos) {
    if (node == nullptr) {
      return {parent, node};
    }
    int64_t left_size = Size(node->left);
    if (pos == left_size) {
      return {parent, node};
    }
    if (pos < left_size) {
      return Find(node, node->left, pos);
    }
    return Find(node, node->right, pos - left_size - 1);
  }

  Node* Merge(Node* first, Node* second) {
    if (first == nullptr) {
      return second;
    }
    if (second == nullptr) {
      return first;
    }
    if (first->priority > second->priority) {
      first->right = Merge(first->right, second);
      Update(first);
      return first;
    }
    second->left = Merge(first, second->left);
    Update(second);
    return second;
  }

  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
  }

  std::pair<Node*, Node*> Split(Node* node, int64_t pos) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    int64_t left_size = Size(node->left);
    if (pos <= left_size) {
      std::pair<Node*, Node*> temp = Split(node->left, pos);
      Node* left = temp.first;
      Node* right = temp.second;
      node->left = right;
      Update(node);
      return {left, node};
    }
    std::pair<Node*, Node*> temp = Split(node->right, pos - left_size - 1);
    Node* left = temp.first;
    Node* right = temp.second;
    node->right = left;
    Update(node);
    return {node, right};
  }
  int GetMin(Node* node) {
    if (node == nullptr) {
      return 1000000000 + 1;
    }
    return node->min;
  }

  void Update(Node* node) {
    if (node == nullptr) {
      return;
    }
    node->size = 1 + Size(node->left) + Size(node->right);
    node->min = std::min(node->value,
                         std::min(GetMin(node->right), GetMin(node->left)));
  }

  int64_t Size(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->size;
  }

  Node* root_;
  std::mt19937 gen_;
  std::uniform_int_distribution<int64_t> distribution_;
};

int main() {
  Treap* treap = new Treap;
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    char comm;
    int l;
    int r;
    std::cin >> comm >> l >> r;
    if (comm == '+') {
      if (l == 0) {
        treap->Insert(0, r);
      } else {
        treap->Insert(l, r);
      }
    } else {
      std::cout << treap->Min(l - 1, r - 1) << '\n';
    }
  }
  delete treap;
}
