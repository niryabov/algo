#include <iostream>

struct Node {
  int value;
  Node* next;
};

struct Stack {
  Node* last = nullptr;
  int size = 0;

  void Push(int val) {
    Node* temp = new Node;
    temp->value = val;
    temp->next = last;
    last = temp;
    size++;
  }

  int Pop() {
    if (last == nullptr) {
      return -1;
    }
    Node* temp = last;
    int val = temp->value;
    last = last->next;
    delete temp;
    size--;
    return val;
  }

  void Clear() {
    while (last != nullptr) {
      Node* temp = last;
      last = last->next;
      delete temp;
    }
    size = 0;
  }

  int Size() { return size; }

  int Back() {
    if (last == nullptr) {
      return -1;
    }
    return last->value;
  }

  void Delete() {
    while (last != nullptr) {
      Node* temp = last;
      last = last->next;
      delete temp;
    }
    delete last;
  }
};

struct StackMin {
  Stack* stk = new Stack;
  Stack* min = new Stack;

  void Push(int val) {
    stk->Push(val);
    if (min->Size() == 0) {
      min->Push(val);
    } else {
      if (min->Back() >= val) {
        min->Push(val);
      } else {
        min->Push(min->Back());
      }
    }
  }
  int Pop() {
    min->Pop();
    return stk->Pop();
  }

  int Back() { return stk->Back(); }

  int Size() { return stk->Size(); }

  int Min() { return min->Back(); }

  void Clear() {
    stk->Clear();
    min->Clear();
  }
  void Delete() {
    stk->Delete();
    min->Delete();
    delete min;
    delete stk;
  }
};

struct Queue {
  StackMin* entry = new StackMin;
  StackMin* out = new StackMin;

  void Push(int n) { entry->Push(n); }
  void Shift() {
    while (entry->Size() != 0) {
      out->Push(entry->Pop());
    }
  }
  int Pop() {
    if (out->Size() == 0) {
      Shift();
    }
    return out->Pop();
  }
  int Size() { return out->Size() + entry->Size(); }

  int Front() {
    if (out->Size() == 0) {
      Shift();
    }
    return out->Back();
  }

  int Min() {
    if (out->Size() == 0) {
      return entry->Min();
    }
    if (entry->Size() == 0) {
      return out->Min();
    }
    return std::min(out->Min(), entry->Min());
  }

  void Clear() {
    out->Clear();
    entry->Clear();
  }
  void Delete() {
    out->Delete();
    entry->Delete();
    delete out;
    delete entry;
  }
};
void Command(const std::string kComm, Queue* stk) {
  if (kComm == "enqueue") {
    int val;
    std::cin >> val;
    stk->Push(val);
    std::cout << "ok" << std::endl;
  } else if (kComm == "dequeue") {
    if (stk->Size() != 0) {
      std::cout << stk->Pop() << std::endl;
    } else {
      std::cout << "error" << std::endl;
    }
  } else if (kComm == "front") {
    if (stk->Size() != 0) {
      std::cout << stk->Front() << std::endl;
    } else {
      std::cout << "error" << std::endl;
    }
  } else if (kComm == "size") {
    std::cout << stk->Size() << std::endl;
  } else if (kComm == "clear") {
    stk->Clear();
    std::cout << "ok" << std::endl;
  } else if (kComm == "min") {
    if (stk->Size() != 0) {
      std::cout << stk->Min() << std::endl;
    } else {
      std::cout << "error" << std::endl;
    }
  }
}

int main() {
  int n;
  std::cin >> n;
  Queue* que = new Queue;
  for (int i = 0; i < n; i++) {
    std::string comm;
    std::cin >> comm;
    Command(comm, que);
  }
  que->Delete();
  delete que;
}
