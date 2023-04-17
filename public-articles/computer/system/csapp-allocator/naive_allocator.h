#pragma once

#include <stdio.h>

namespace csapp {

class NaiveAllocator {
 public:
  NaiveAllocator() = default;

  NaiveAllocator(const NaiveAllocator&) = delete;
  NaiveAllocator& operator=(const NaiveAllocator&) = delete;

 private:
  /**
   * @brief Initialize the memory system model
   * @return If successful, return true. Else return false.
   */
  bool InitInternal();

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

 private:
  // Basic constants

  /// Word and header/foot size(bytes)
  static constexpr int kWordSize = 4;

  /// Double-word size(bytes)
  static constexpr int kDoubleWordSize = 8;

  /// Chunk size(4K)
  static constexpr int kChunkSize = (1 << 12);
};

}  // namespace csapp
