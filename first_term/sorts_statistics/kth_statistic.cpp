#include <iostream>
long long Partition(long long* arr, long long left_border,
                    long long right_border, long long pivot) {
  long long left = left_border;
  long long right = right_border - 1;
  long long i = left_border;
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
  return left;
}

long long Pivot(long long* arr, long long left, long long right) {
  return arr[(left + right) / 2];
}
long long QuickSelect(long long* arr, long long left, long long right,
                      long long k) {
  while (true) {
    if (left == right) {
      return left;
    }
    long long pivot_i = Pivot(arr, left, right);
    pivot_i = Partition(arr, left, right, pivot_i);
    if (k == pivot_i) {
      return k;
    }
    if (k < pivot_i) {
      right = pivot_i;
    } else {
      left = pivot_i + 1;
    }
  }
}
int main() {
  long long n;
  long long k;
  std::cin >> n >> k;
  long long* a = new long long[n];
  std::cin >> a[0] >> a[1];
  for (long long i = 2; i < n; i++) {
    a[i] = (a[i - 1] * 123 + a[i - 2] * 45) % (10000000 + 4321);
  }
  std::cout << a[QuickSelect(a, 0, n, k - 1)];
  delete[] a;
}
