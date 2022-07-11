#include <utility>
#include <vector>
#include <queue>

// total numbers of nodes
constexpr int N{32};

// head of node
constexpr int kHead{0};

// tail of node
constexpr int kTail{-1};

// adjacency list
int ver[N];
int edge[N];
int next[N];
int head[N] = {kTail};  // 注意初始化值 可以-1/0 遍历的时候需要根据tail(0/-1)进行
int tot{-1};  // index of edge list

void add(int x, int y, int w) {
  ver[++tot] = y; edge[tot] = w;
  next[tot] = head[x]; head[x] = tot;
}

// visited nodes
int vis[N] = {0};

namespace dfs::base {

void dfs(int x) {
  vis[x] = 1;
  for (int i = head[x]; i != kTail ; i = next[i]) {
    int y = ver[i];
    if (vis[y]) continue;
    dfs(y);
  }
}

} // namespacd dfs::base

namespace dfs::sequence {

// dfs sequence
int s[N];

// index of dfs sequence
int m{-1};

void dfs(int x) {
  s[++m] = x;
  vis[x] = 1;
  for (int i = head[x]; i != kTail ; i = next[i]) {
    int y = ver[i];
    if (vis[y]) continue;
    dfs(y);
  }
  s[++m] = x;
}

}  // namespace dfs::sequence

namespace dfs::depth {

int d[N] = {1}; // zero is also ok

void dfs(int x) {
  vis[x] = 1;
  for (int i = head[x]; i != kTail ; i = next[i]) {
    int y = ver[i];
    if (vis[y]) continue;
    d[y] = d[x] + 1;  // top-down operation is here.
    dfs(y);
  }
}

}  // namespace dfs::depth

namespace dfs::size {

// size of a tree node
int size[N] = {0};

void dfs(int x) {
  vis[x] = 1;
  size[x] = 1;
  for (int i = head[x]; i != kTail ; i = next[i]) {
    int y = ver[i];
    if (vis[y]) continue;
    dfs(y);
    size[x] += size[y];
  }
}

}  // namespace dfs::size

namespace dfs::cg {

// size of a tree node
int size[N] = {0};
std::vector<int> ans;

void dfs(int x) {
  vis[x] = 1;
  size[x] = 1;
  int max_part{0}; // max subtree of x
  for (int i = head[x]; i != kTail ; i = next[i]) {
    int y = ver[i];
    if (vis[y]) continue;
    dfs(y);
    size[x] += size[y];
    max_part = std::max(max_part, size[y]);
  }
  max_part = std::max(max_part, N - size[x]);
  if (max_part <= N/2) ans.emplace_back(x);
}

}  // namespace dfs::cg

namespace dfs::cc {

// total numbers of connected component
int cnt{0};

void dfs(int x) {
  vis[x] = cnt;
  for (int i = head[x]; i != kTail; i = next[i]) {
    int y = ver[i];
    if (vis[y]) continue;
    dfs(y);
  }
}

void cc() {
  for (int i = 0; i < N; ++i) {
    if (!vis[i]) {
      cnt++;
      dfs(i);
    }
  }
}

}  // namespace dfs::cc

namespace bfs::base {

void bfs() {
  std::queue<int> que;
  que.push(kHead);
  vis[kHead] = 1;
  while (!que.empty()) {
    int x = que.front(); que.pop();
    for (int i = head[x]; i != kTail; i = next[i]) {
      int y = ver[i];
      if (vis[y]) continue;
      vis[y] = 1;
      que.push(y);
    }

  }
}

}  // namespace bfs::base

namespace bfs::depth {

int d[N] = {0};

void depth() {
  std::queue<int> que;
  que.push(kHead);
  d[kHead] = 1;
  while (!que.empty()) {
    int x = que.front(); que.pop();
    for (int i = head[x]; i != kTail; i = next[i]) {
      int y = ver[i];
      if (d[y]) continue;
      d[y] = d[x] + 1;
      que.push(y);
    }
  }
}

}  // namespace bfs::depth

namespace bfs::topsort {

int deg[N] = {0};  // in-degree
std::vector<int> top_seq;

int ver_num{0}; // vertex num

void add(int x, int y, int w) {
  ver[++tot] = y; edge[tot] = w;
  next[tot] = head[x]; head[x] = tot;
  deg[y]++;
}

void topsort() {
  std::queue<int> que;
  for (int i = 0; i < ver_num; ++i) {
    if (!deg[i]) {
      que.push(i);
      top_seq.emplace_back(i);
    }
  }
  while (!que.empty()) {
    int x = que.front(); que.pop();
    for (int i = head[x]; i != kTail; i = next[i]) {
      int y = ver[i];
      deg[y]--;
      if (!deg[y]) {
        que.push(y);
        top_seq.emplace_back(y);
      }
    }
  }
}

bool is_cyclic() {
  topsort();
  return static_cast<int>(top_seq.size()) == ver_num;
}

}  // namespace bfs::topsort

int main(void) {
  return 0;
}
