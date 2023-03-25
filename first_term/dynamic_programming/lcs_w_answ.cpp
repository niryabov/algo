#include <iostream>
#include <vector>

void ShowSubSeq(int first, int second,
                std::vector<std::vector<std::pair<int, int>>>& indexes,
                const int* k_seq1) {
  if (first == 0 || second == 0) {
    return;
  }
  if (indexes[first][second].first == first - 1 &&
      indexes[first][second].second == second - 1) {
    ShowSubSeq(first - 1, second - 1, indexes, k_seq1);
    std::cout << k_seq1[first - 1] << ' ';
  } else if (indexes[first][second].first == first - 1 &&
             indexes[first][second].second == second) {
    ShowSubSeq(first - 1, second, indexes, k_seq1);
  } else {
    ShowSubSeq(first, second - 1, indexes, k_seq1);
  }
}

void Subseq(const int* k_seq1, int first, const int* k_seq2, int second) {
  std::vector<std::vector<int>> dp(first + 1, std::vector<int>(second + 1, 0));
  std::vector<std::vector<std::pair<int, int>>> indexes(
      first + 1, std::vector<std::pair<int, int>>(second + 1, {0, 0}));
  for (int i = 1; i <= first; ++i) {
    for (int j = 1; j <= second; ++j) {
      if (k_seq1[i - 1] == k_seq2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
        indexes[i][j] = {i - 1, j - 1};
      } else if (dp[i - 1][j] >= dp[i][j - 1]) {
        dp[i][j] = dp[i - 1][j];
        indexes[i][j] = {i - 1, j};
      } else {
        dp[i][j] = dp[i][j - 1];
        indexes[i][j] = {i, j - 1};
      }
    }
  }
  ShowSubSeq(first, second, indexes, k_seq1);
}

int main() {
  int first;
  std::cin >> first;
  int* seq1 = new int[first];
  for (int i = 0; i < first; ++i) {
    std::cin >> seq1[i];
  }
  int second;
  std::cin >> second;
  int* seq2 = new int[second];
  for (int i = 0; i < second; ++i) {
    std::cin >> seq2[i];
  }

  Subseq(seq1, first, seq2, second);

  delete[] seq1;
  delete[] seq2;
}
