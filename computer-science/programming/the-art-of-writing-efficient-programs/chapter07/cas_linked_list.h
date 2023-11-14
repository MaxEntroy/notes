#pragma once

#include <atomic>
#include <new>

template<typename T>
struct Node {
  T data = 0;
  Node* next = nullptr;

  explicit Node(const T& data) : data(data) , next(nullptr) {}
};

template<typename T>
class CasLinkedList {
 public:
  void PushFront(const T& data) {
    auto* new_node = new (std::nothrow) Node(data);
    if (!new_node) [[unlikely]] {
      return;
    }

    new_node->next = head_.load(std::memory_order_relaxed);
    while (!head_.compare_exchange_weak(new_node->next, new_node,
                                        std::memory_order_release,
                                        std::memory_order_relaxed)) {}
  }

  void PopFront() {
    auto old_head = head_.load(std::memory_order_relaxed);
    while (!head_.compare_exchange_weak(old_head, old_head->next,
                                        std::memory_order_release,
                                        std::memory_order_relaxed)) {}
    delete old_head;
  }

 private:
  std::atomic<Node<T>*> head_;
};
