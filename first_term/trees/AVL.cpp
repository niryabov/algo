#include <iostream>

class Tree {
 public:
  int value;
  int childs;
  int height;
  Tree* left;
  Tree* right;

  Tree(int value) {
    this->value = value;
    this->left = nullptr;
    this->right = nullptr;
    this->height = 1;
    this->childs = 1;
  }
  Tree* Insert(Tree* tree, int value) {
    if (tree == nullptr) {
      return new Tree(value);
    }

    tree->childs++;
    if (value < tree->value) {
      tree->left = Insert(tree->left, value);
    } else {
      tree->right = Insert(tree->right, value);
    }
    return Balance(tree);
  }

  int FindClosest(Tree* tree, int val, int border) {
    if (tree == nullptr) {
      return border;
    }

    if (tree->value == val) {
      return val;
    }
    if (tree->value < val) {
      return FindClosest(tree->right, val, border);
    }
    if (tree->value > val) {
      border = tree->value;
      return FindClosest(tree->left, val, border);
    }
    return border;
  }
  ~Tree() {}
  void DeleteTree(Tree* tree) {
    if (tree->right != nullptr) {
      DeleteTree(tree->right);
    }
    if (tree->left != nullptr) {
      DeleteTree(tree->left);
    }
    delete tree;
  }

 private:
  int GetHeight(Tree* tree) { return tree == nullptr ? 0 : tree->height; }

  int GetChilds(Tree* tree) { return tree == nullptr ? 0 : tree->childs; }

  int GetBalance(Tree* tree) {
    return tree == nullptr ? 0 : GetHeight(tree->right) - GetHeight(tree->left);
  }
  void SetNewHeight(Tree* tree) {
    int l_height = GetHeight(tree->left);
    int r_height = GetHeight(tree->right);
    tree->height = l_height > r_height ? l_height + 1 : r_height + 1;
  }

  void SetNewChilds(Tree* tree) {
    tree->childs = GetChilds(tree->left) + GetChilds(tree->right) + 1;
  }

  Tree* RotateRight(Tree* tree) {
    Tree* temp = tree->left;
    tree->left = temp->right;
    temp->right = tree;
    SetNewChilds(tree);
    SetNewHeight(tree);
    SetNewHeight(temp);
    SetNewChilds(temp);
    return temp;
  }
  Tree* RotateLeft(Tree* tree) {
    Tree* temp = tree->right;
    tree->right = temp->left;
    temp->left = tree;
    SetNewChilds(tree);
    SetNewHeight(tree);
    SetNewHeight(temp);
    SetNewChilds(temp);
    return temp;
  }
  Tree* Balance(Tree* tree) {
    SetNewChilds(tree);
    SetNewHeight(tree);
    if (GetBalance(tree) == 2) {
      if (GetBalance(tree->right) < 0) {
        tree->right = RotateRight(tree->right);
      }
      return RotateLeft(tree);
    }
    if (GetBalance(tree) == -2) {
      if (GetBalance(tree->left) > 0) {
        tree->left = RotateLeft(tree->left);
      }
      return RotateRight(tree);
    }
    return tree;
  }
};

int main() {
  int n;
  std::cin >> n;
  int lastans = 0;
  char op;
  int value;
  Tree* tree = new Tree(-1);
  for (int i = 0; i < n; i++) {
    std::cin >> op >> value;
    if (op == '+') {
      tree = tree->Insert(tree, (value + lastans) % 1000000000);
      lastans = 0;
    }
    if (op == '?') {
      lastans = tree->FindClosest(tree, value, -1);
      std::cout << lastans << '\n';
    }
  }

  tree->DeleteTree(tree);
}
