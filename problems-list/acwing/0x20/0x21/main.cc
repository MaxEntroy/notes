#include <utility>
#include <vector>

// total numbers of nodes
#define N 32

// adjacency list
int ver[N];
int edge[N];
int next[N];
int head[N] = {-1};
int tot{-1};

// visited nodes
int v[N] = {0};

namespace base {

void dfs(int x) {
  v[x] = 1;
  for (int i = head[x]; i ; i = next[i]) {
    int y = ver[i];
    if (v[y]) continue;
    dfs(y);
  }
}

} // namespacd base

namespace sequence {

// dfs sequence
int s[N];

// index of dfs sequence
int m{-1};

void dfs(int x) {
  s[++m] = x;
  v[x] = 1;
  for (int i = head[x]; i ; i = next[i]) {
    int y = ver[i];
    if (v[y]) continue;
    dfs(y);
  }
  s[++m] = x;
}

}  // namespace sequence

namespace depth {

int d[N] = {1}; // zero is also ok

void dfs(int x) {
  v[x] = 1;
  for (int i = head[x]; i ; i = next[i]) {
    int y = ver[i];
    if (v[y]) continue;
    d[y] = d[x] + 1;  // top-down operation is here.
    dfs(y);
  }
}

}  // namespace depth

namespace size {

// size of a tree node
int size[N] = {0};

void dfs(int x) {
  v[x] = 1;
  size[x] = 1;
  for (int i = head[x]; i ; i = next[i]) {
    int y = ver[i];
    if (v[y]) continue;
    dfs(y);
    size[x] += size[y];
  }
}

}  // namespace size

namespace cg {

// size of a tree node
int size[N] = {0};
std::vector<int> ans;

void dfs(int x) {
  v[x] = 1;
  size[x] = 1;
  int max_part{0}; // max subtree of x
  for (int i = head[x]; i ; i = next[i]) {
    int y = ver[i];
    if (v[y]) continue;
    dfs(y);
    size[x] += size[y];
    max_part = std::max(max_part, size[y]);
  }
  max_part = std::max(max_part, N - size[x]);
  if (max_part <= N/2) ans.emplace_back(x);
}

}  // namespace cg

namespace cc {

// total numbers of connected component
int cnt{0};

void dfs(int x) {
  v[x] = cnt;
  for (int i = head[x]; i ; i = next[i]) {
    int y = ver[i];
    if (v[y]) continue;
    dfs(y);
  }
}

void cc() {
  for (int i = 0; i < N; ++i) {
    if (!v[i]) {
      cnt++;
      dfs(i);
    }
  }
}

}  // namespace cc

int main(void) {
  return 0;
}
