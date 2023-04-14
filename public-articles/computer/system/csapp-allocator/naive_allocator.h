#pragma once

#include <stdio.h>

namespace csapp {

class NaiveAllocator {
 public:
  NaiveAllocator() = default;

  NaiveAllocator(const NaiveAllocator&) = delete;
  NaiveAllocator& operator=(const NaiveAllocator&) = delete;

  /**
   * @brief Initialize the memory system model
   * @return If successful, return true. Else return false.
   */
  bool Init();

  /**
   * @brief Simple model of the sbrk function.
   * Extends the heap by incr bytes and returns the start address of the new area.
   * In this model, the heap cannot be shrunk.
   * @param incr
   * @return If successful, return a pointer to allocated memory. Else return false.
   */
  void* Sbrk(int incr);

 private:
  /// Points to first byte of heap
  char* mem_heap = nullptr;

  /// Points to last byte of heap plus 1
  char* mem_brk = nullptr;

  /// Max legal heap addr plus 1
  char* mem_max_addr = nullptr;

  /// The maximum block size
  static constexpr size_t kMaxHeap = (1 << 20);
};

}  // namespace csapp
