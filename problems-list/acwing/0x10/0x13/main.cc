#include <stdio.h>

#include <iostream>
#include <list>
#include <vector>

namespace method1 {

//void add(int v1, int v2, int w) {
//  ver[++tot] = v2;
//  edge[tot] = w;
//  next[tot] = head[v1];
//  head[v1] = tot;
//}

class Graph {
 public:
  explicit Graph(int ver_num) { adj_list_.head.resize(ver_num, kTail); }

  Graph(const Graph&) = delete;
  Graph& operator=(const Graph&) = delete;

  void AddEdge(int v1, int v2, int w) {
    adj_list_.edges.emplace_back(v2, w, adj_list_.head[v1]);
    adj_list_.head[v1] = adj_list_.edges.size() - 1;
  }

  void TraverseEdge() const {
    for (int ver = 0, sz = adj_list_.head.size(); ver < sz; ++ver) {
      int cur = adj_list_.head[ver];
      while (cur != kTail) {
        const auto& e = adj_list_.edges[cur];
        std::cout << "(" << ver << "," << e.node << "," << e.weight << ")" << std::endl;;
        cur = e.next;
      }
    }
  }

 private:
  static constexpr int kTail{-1};
  struct Edge {
    int node{-1};
    int weight{0};
    int next{-1};

    Edge(int n, int w, int ne) : node(n), weight(w), next(ne) {}
  };
  struct AdjList {
    std::vector<Edge> edges;
    std::vector<int> head;
  };
  AdjList adj_list_;
};

void test() {
  int n;
  std::cin >> n;
  Graph g(n);
  int v1, v2, w;
  while (std::cin >> v1 >> v2 >> w) {
    g.AddEdge(v1, v2, w);
  }
  g.TraverseEdge();
}

} // namespace method1

namespace method2 {

class Graph {
 public:
  explicit Graph(int ver_num) { adj_list_.resize(ver_num); }

  Graph(const Graph&) = delete;
  Graph& operator=(const Graph&) = delete;

  void AddEdge(int v1, int v2, int w) {
    adj_list_[v1].emplace_front(v2, w);
  }

  void TraverseEdge() const {
    int v{0};
    for (const auto& edges : adj_list_) {
      for (const auto& edge : edges) {
        std::cout << "(" << v << "," << edge.node << "," << edge.weight << ")" << std::endl;
      }
      ++v;
    }
  }

 private:
  struct Edge {
    int node{-1};
    int weight{0};

    Edge(int n, int w) : node(n), weight(w) {}
  };
  using AdjList = std::vector<std::list<Edge> >;
  AdjList adj_list_;
};

void test() {
  int n;
  std::cin >> n;
  Graph g(n);
  int v1, v2, w;
  while (std::cin >> v1 >> v2 >> w) {
    g.AddEdge(v1, v2, w);
  }
  g.TraverseEdge();
}

}  // namespace method2

int main(void) {
  method1::test();
  //method2::test();
  return 0;
}
