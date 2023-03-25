#include <cstdlib>
#include <iostream>
#include <optional>
#include <utility>
using namespace std;
class Treap {
  struct Node {
    Node(int x, int y) : x(x), y(y), left(nullptr), right(nullptr) {}
    int x;
    int y;
    int childrens{1};
    Node* left;
    Node* right;
  };

 public:
  Treap() : root_(nullptr){};
  ~Treap() { Clear(root_); }

  void Insert(int value) {
    if (Find(value)) {
      return;
    }
    int y = rand();
    Node* node = new Node(value, y);
    pair<Node*, Node*> temp = Split(root_, value);
    Node* first = temp.first;
    Node* second = temp.second;
    root_ = Merge(Merge(first, node), second);
  }
  bool Find(int value) { return Find(nullptr, root_, value).second != nullptr; }

  void Erase(int value) {
    if (!Find(value)) {
      return;
    }
    pair<Node*, Node*> temp = Split(root_, value);
    Node* left_with_value = temp.first;
    Node* right = temp.second;
    pair<Node*, Node*> temp_new = Split(left_with_value, value - 1);
    Node* left = temp_new.first;
    Node* value_tree = temp_new.second;
    delete value_tree;
    root_ = Merge(left, right);
  }

  string Next(int value) {
    optional<int> temp = Next(root_, value, nullopt);
    return temp.has_value() ? to_string(temp.value()) : "none";
  }
  string Prev(int value) {
    optional<int> temp = Prev(root_, value, nullopt);
    return temp.has_value() ? to_string(temp.value()) : "none";
  }
  string Kth(int value) {
    optional<int> temp = Kth(root_, value);
    return temp.has_value() ? to_string(temp.value()) : "none";
  }

  void EraseFind(int value) {
    if (root_ == nullptr) {
      return;
    }
    pair<Node*, Node*> temp = Find(nullptr, root_, value);
    Node* parent = temp.first;
    Node* value_node = temp.second;
    if (value_node == nullptr) {
      return;
    }
    if (parent == nullptr && value_node != nullptr) {
      root_ = Merge(root_->left, root_->right);
    } else {
      ((parent->left == value_node) ? parent->left : parent->right) =
          Merge(value_node->left, value_node->right);
    }

    delete value_node;
  }

 private:
  optional<int> Kth(Node* node, int value) {
    if (node == nullptr) {
      return nullopt;
    }
    int childs = GetChildrens(node->left);
    if (childs == value) {
      return node->x;
    }
    if (childs > value) {
      return Kth(node->left, value);
    }
    if (childs < value) {
      return Kth(node->right, (value - childs - 1));
    }
    return nullopt;
  }

  optional<int> Next(Node* tree, int value, optional<int> border) {
    if (tree == nullptr) {
      return border;
    }

    if (tree->x == value) {
      return value;
    }
    if (tree->x < value) {
      return Next(tree->right, value, border);
    }
    if (tree->x > value) {
      border = tree->x;
      return Next(tree->left, value, border);
    }
    return border;
  }

  optional<int> Prev(Node* tree, int value, optional<int> border) {
    if (tree == nullptr) {
      return border;
    }

    if (tree->x == value) {
      return value;
    }
    if (tree->x > value) {
      return Prev(tree->left, value, border);
    }
    if (tree->x < value) {
      border = tree->x;
      return Prev(tree->right, value, border);
    }
    return border;
  }
  pair<Node*, Node*> Find(Node* parent, Node* node, int value) {
    if (node == nullptr) {
      return {parent, node};
    }
    if (value == node->x) {
      return {parent, node};
    }
    if (value <= node->x) {
      return Find(node, node->left, value);
    }
    return Find(node, node->right, value);
  }

  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    if (node->left != nullptr) {
      Clear(node->left);
    }
    if (node->right != nullptr) {
      Clear(node->right);
    }
    delete node;
  }

  Node* Merge(Node* first, Node* second) {
    if (first == nullptr) {
      return second;
    }
    if (second == nullptr) {
      return first;
    }
    if (first->y > second->y) {
      first->right = Merge(first->right, second);
      SetNewChildren(first);
      return first;
    }
    second->left = Merge(first, second->left);
    SetNewChildren(second);
    return second;
  }
  int GetChildrens(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->childrens;
  }
  int SetNewChildren(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    node->childrens = GetChildrens(node->left) + GetChildrens(node->right) + 1;
    return node->childrens;
  }
  pair<Node*, Node*> Split(Node* node, int key) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    if (node->x > key) {
      pair<Node*, Node*> temp = Split(node->left, key);
      Node* left = temp.first;
      Node* right = temp.second;
      node->left = right;
      SetNewChildren(node);
      return {left, node};
    }
    pair<Node*, Node*> temp = Split(node->right, key);
    Node* left = temp.first;
    Node* right = temp.second;
    node->right = left;
    SetNewChildren(node);
    return {node, right};
  }

  Node* root_;
};

int main() {
  Treap* a = new Treap;
  int value;
  string op;
  while (cin >> op >> value) {
    if (op == "insert") {
      a->Insert(value);
    }
    if (op == "delete") {
      a->Erase(value);
    }
    if (op == "exists") {
      cout << (a->Find(value) ? "true" : "false") << '\n';
    }
    if (op == "next") {
      cout << a->Next(value) << '\n';
    }
    if (op == "prev") {
      cout << a->Prev(value) << '\n';
    }
    if (op == "kth") {
      cout << a->Kth(value) << '\n';
    }
  }
  delete a;
}
