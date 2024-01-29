#include <iostream>
#include <vector>

constexpr int N{32};

int chosen[N] = {0};
bool vis[N] = {false};

namespace enumeration_with_permutation {

int n{0};

void dfs(int pos) {
  if (pos == n + 1) {
    for (int i = 1; i <= n; ++i)
      std::cout << chosen[i] << " ";
    std::cout << std::endl;
    return ;
  }

  for (int i = 1; i <= n; ++i) {
    // pruning
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

namespace enumeration_with_combinations {

int n{0};
int m{0};

void dfs(int pos) {
  if (pos == m + 1) {
    for (int i = 1; i <= m; ++i)
      std::cout << chosen[i] << " ";
    std::cout << std::endl;
    return ;
  }

  for (int i = 1; i <= n; ++i) {
    // pruning
    if (vis[i]) continue;

    // try current
    chosen[pos] = i;
    vis[i] = true;

    // solve subproblem recursionly
    dfs(pos + 1);

    // backtracking
    chosen[pos] = 0;
    vis[i] = false;
  }
}

}  // namespace enumeration_with_combinations

namespace enumeration_with_exponent {

int n{0};
std::vector<int> chosen;

void dfs(int x) {
  if (x == n + 1) {
    if (!chosen.empty()) {
        for (const auto& val : chosen) std::cout << val << " ";
        std::cout << std::endl;
    }
    return;
  }

  // not choose
  {
    dfs(x + 1);
  }

  // choose
  {
    chosen.emplace_back(x);
    dfs(x + 1);
    chosen.pop_back();
  }
}

}  // namespace enumeration_with_exponent

int main(void) {
  return 0;
}
