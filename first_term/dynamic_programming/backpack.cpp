#include <iostream>
#include <vector>

void ShowBackPack(int i, int j, const std::vector<std::vector<int>> kDp,
                  const int* masses) {
  if (kDp[i][j] == 0) {
    return;
  }
  if (kDp[i - 1][j] == kDp[i][j]) {
    ShowBackPack(i - 1, j, kDp, masses);
  } else {
    ShowBackPack(i - 1, j - masses[i - 1], kDp, masses);
    std::cout << i << '\n';
  }
}

void Backpack(int count, int capacity, const int* masses, const int* values) {
  std::vector<std::vector<int>> dp(count + 1,
                                   std::vector<int>(capacity + 1, 0));

  for (int i = 1; i <= count; ++i) {
    for (int j = 1; j <= capacity; ++j) {
      if (j >= masses[i - 1]) {
        dp[i][j] = std::max(dp[i - 1][j],
                            dp[i - 1][j - masses[i - 1]] + values[i - 1]);
      } else {
        dp[i][j] = dp[i - 1][j];
      }
    }
  }
  ShowBackPack(count, capacity, dp, masses);
}

int main() {
  int count;
  int capacity;
  std::cin >> count >> capacity;
  int* masses = new int[count];
  int* values = new int[count];
  for (int i = 0; i < count; ++i) {
    std::cin >> masses[i];
  }
  for (int i = 0; i < count; ++i) {
    std::cin >> values[i];
  }
  Backpack(count, capacity, masses, values);
  delete[] masses;
  delete[] values;
}
