#include <iostream>
#include <vector>

int64_t Subseq1(const int64_t* seq, int64_t n) {
  std::vector<std::vector<int64_t>> dp(n, std::vector<int64_t>(n, 0));
  for (int64_t i = 0; i < n; ++i) {
    dp[0][i] = 1;
  }
  for (int64_t j = 1; j < n; ++j) {
    for (int64_t i = 0; i + j < n; i++) {
      if (seq[i] == seq[i + j]) {
        dp[j][i] = (dp[j - 1][i] + dp[j - 1][i + 1] + 1);
      } else {
        dp[j][i] = (dp[j - 1][i + 1] + dp[j - 1][i] -
                    (j - 2 >= 0 ? dp[j - 2][i + 1] : 0));
      }
      dp[j][i] = dp[j][i] >= 0 ? dp[j][i] % 1000000000
                               : 1000000000 + dp[j][i] % 1000000000;
    }
  }
  return dp[n - 1][0];
}

int main() {
  int64_t first;
  std::cin >> first;
  int64_t* seq1 = new int64_t[first];
  for (int64_t i = 0; i < first; ++i) {
    std::cin >> seq1[i];
  }
  std::cout << Subseq1(seq1, first);
  delete[] seq1;
}
