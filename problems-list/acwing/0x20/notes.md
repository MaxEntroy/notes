## 0x20 搜索

核心关注点：如何对问题的状态空间进行遍历与映射

### 0x21 树与图的遍历

#### 基础dfs算法

深度优先遍历，就是在每个点x上面对多条分支时，任意选一条边走下去，执行递归，直至回溯到点x后，再考虑走向其他的边。

公共结构

```cpp
// adjacency list
int ver[N];
int edge[N];
int next[N];
int head[N] = {-1};
int tot{-1};

// visited nodes
int v[N] = {0};
```

遍历算法

```cpp
void dfs(int x) {
  v[x] = 1;
  for (int i = head[x]; i ; ++i) {
    int y = ver[i];
    if (v[y]) continue;
    dfs(y);
  }
}
```

- 时间复杂度：O(M+N)
- 边界条件：当一个点的所有边均访问过时，递归返回
- 递归方式：top-down(根据操作操作在递归前还是递归后)

#### 树的dfs序

在对树进行dfs时，在刚入递归后和递归返回前，各记录一次该点的编号，最后产生的长度为2N的节点序列，就称为树的dfs序。
这里我强调一点，**本小节所有关于树的讨论，都是一般意义上树，不特指二叉树。所以会有visited数组来标记是否已经访问过。边有向时，倾向于理解为树，无向时理解为图**

```cpp
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
```

我们可以通过dfs序把子树统计转化为序列上的区间统计，本质是将非线性结构，转为为线性结构

#### 树的深度

树中各个节点的深度，是一种top-down manner统计信息的。
- 已知根节点的深度为1(0)
- 若节点x的深度为d[x]，则它的孩子节点y，深度d[y] = d[x] + 1
- top-down manner的核心操作在下一次dfs之前

```cpp
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
```

#### 树的大小

当然也有许多信息是bottom-up manner统计的，比如以节点x为根的子树大小size[x]
- 对于叶子节点x，size[x] = 1
- 对于非叶子节点x
  - 设其孩子节点为y1,y2,...yk,并且其子树大小为size[y1],size[y2],...size[yk]
  - 则以x为根的子树大小为：size[x] = size[y1] + size[y2] + ... + size[yk] + 1


#### 树的重心

这个算法在树的大小基础上，具体定义如下
- 对于某一个节点x，如果我们把他从树中删除，那么原来的一棵树会分成若干个不相连的部分
- 其中每一个部分都是一颗子树，设max_part(x)表示子树中size最大的那棵
- 对于所有节点x，max_part(x)最小的节点，即为该树的重心

可做如下理解：树的重心节点，它的最大子树是所有节点最大子树最小的一个。
- 一个节点的最大子树，可以理解为移除这个节点后，整个树的划分
- 最大子树越小，证明整个树划分的越平衡。

重心具备如下性质：
性质1：某个点是树的重心等价于它最大子树大小不大于整棵树大小的一半
性质2：树至多有两个重心

参考[树的重心](https://zhuanlan.zhihu.com/p/357938161)

```cpp
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
  max_part = std::max(max_part, N - size[x]);  // for direct graph
  if (max_part <= N/2) ans.emplace_back(x);
}
```

有一点需要注意，就是```max_part = std::max(max_part, N - size[x])```这里对于有向图的处理，否则所有叶子节点max_part均为0，那么最后叶子节点为重心，显然不对。
这里可以理解为节点x上面的子树

#### 图的联通分量划分

上面代码从x开始一次遍历，就能访问x所能到达的所有的点与边。因此通过多次dfs，可以划分出一张无向图中的各个联通分量。

- 增加cnt作为联通分量的标识，也是联通分量的个数

```cpp
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
```

#### 基础bfs算法

广度优先遍历，就是在每个点x上面对多条分支时，将所有直接相邻边全部访问后，再考虑访问其余边。本质是一种基于层次的遍历