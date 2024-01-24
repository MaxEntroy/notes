#pragma once

#include <stdio.h>

namespace csapp {

// It is implemented based on implicit free list.
class NaiveAllocator {
 public:
  // type alias for Byte and Word
  using Byte = char;
  using Word = unsigned int;

  NaiveAllocator() = default;

  NaiveAllocator(const NaiveAllocator&) = delete;
  NaiveAllocator& operator=(const NaiveAllocator&) = delete;

  NaiveAllocator(NaiveAllocator&&) = delete;
  NaiveAllocator& operator=(NaiveAllocator&&) = delete;

  // Before calling Malloc or free functions, the application must initialize the heap
  // by calling the Init function.
  bool Init();

 private:
  // Initialize the memory system model
  bool MemInit();

  // Simple model of the sbrk function.
  // Extends the heap by incr bytes and returns the start address of the new area.
  // In this model, the heap cannot be shrunk.
  void* MemSbrk(int incr);

 private:
  // Points to first byte of heap
  Byte* mem_heap_ = nullptr;

  // Points to last byte of heap plus 1
  Byte* mem_brk_ = nullptr;

  // Max legal heap addr plus 1
  Byte* mem_max_addr_ = nullptr;

  // Points to first byte of heap
  void* mem_heap_prologue_ = nullptr;

  // The maximum block size
  static constexpr size_t kMaxHeap = (1 << 20);

 private:
  // Word and header/foot size(bytes)
  static constexpr int kWordSize = 4;

  // Double-word size(bytes)
  static constexpr int kDoubleWordSize = 8;

  // Extend heap by this amount(bytes)
  static constexpr int kChunkSize = (1 << 12);
};

}  // namespace csapp
