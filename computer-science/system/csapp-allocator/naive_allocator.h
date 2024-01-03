#pragma once

#include <stdio.h>

namespace csapp {

// It is implemented based on implicit free list.
class NaiveAllocator {
 public:
  NaiveAllocator() = default;

  NaiveAllocator(const NaiveAllocator&) = delete;
  NaiveAllocator& operator=(const NaiveAllocator&) = delete;

  NaiveAllocator(NaiveAllocator&&) = delete;
  NaiveAllocator& operator=(NaiveAllocator&&) = delete;

 private:
  // Initialize the memory system model
  bool MemInit();

  // Simple model of the sbrk function.
  // Extends the heap by incr bytes and returns the start address of the new area.
  // In this model, the heap cannot be shrunk.
  void* MemSbrk(int incr);

 private:
  // type alias for byte
  using Byte = char;

  // Points to first byte of heap
  Byte* mem_heap_ = nullptr;

  // Points to last byte of heap plus 1
  Byte* mem_brk_ = nullptr;

  // Max legal heap addr plus 1
  Byte* mem_max_addr_ = nullptr;

  // The maximum block size
  static constexpr size_t kMaxHeap = (1 << 20);

 private:
  /// Word and header/foot size(bytes)
  static constexpr int kWordSize = 4;

  /// Double-word size(bytes)
  static constexpr int kDoubleWordSize = 8;

  /// Extend heap by this amount(bytes)
  static constexpr int kChunkSize = (1 << 12);
};

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// Pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Read and write a word at address p
#define GET(p) (*(unsigned int*)(p))
#define PUT(p, val) (*(unsigned int*)(p) = (val))

// Read the size and allocated bits from address p
#define GET_SIZE(p) (GET(p) & )
#define GET_ALLOC(p) (GET(p) & )

}  // namespace csapp
