#include <iostream>

namespace enumeration_with_permutation {

constexpr int N{32};

int chosen[N] = {0};
bool vis[N] = {false};

int n{0};

void dfs(int pos) {
  if (pos == n + 1) {
    for (int i = 1; i <= n; ++i)
      std::cout << chosen[i] << " ";
    std::cout << std::endl;
    return ;
  }

  for (int i = 1; i <= n; ++i) {
    if (vis[i]) continue;
    // try current
    chosen[pos] = i;
    vis[i] = true;

    // solve subproblem  recursionly
    dfs(pos + 1);

    // backtracking
    chosen[pos] = 0;  // not necessary
    vis[i] = false;
  }
}

}  // enumeration_with_permutation

int main(void) {
  return 0;
}
