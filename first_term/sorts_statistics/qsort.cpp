#include <iostream>
std::pair<int, int> Partition(int* arr, int left_border, int right_border,
                              int pivot) {
  int left = left_border;
  int right = right_border - 1;
  int i = left_border;
  while (i <= right) {
    if (arr[i] == pivot) {
      i++;
    } else if (arr[i] < pivot) {
      std::swap(arr[i], arr[left]);
      left++;
      i++;
    } else {
      std::swap(arr[i], arr[right]);
      right--;
    }
  }
  return {left, right};
}

int Pivot(int* arr, int left, int right) {
  int mid = (left + right) / 2;
  return arr[mid];
}

void QuickSort(int* arr, int left, int right) {
  if (left >= right) {
    return;
  }
  int i = Pivot(arr, left, right);
  std::pair<int, int> par = Partition(arr, left, right, i);
  QuickSort(arr, left, par.first);
  QuickSort(arr, par.second + 1, right);
}
int main() {
  int n;
  std::cin >> n;
  int* a = new int[n];

  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  QuickSort(a, 0, n);
  for (int i = 0; i < n; i++) {
    std::cout << a[i] << ' ';
  }
  delete[] a;
}
