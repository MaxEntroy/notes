#pragma once

#include <stdio.h>

namespace csapp {

// It is implemented based on implicit free list.
class SimpleAllocator {
 public:
  // type alias for Byte and Word
  using Byte = char;
  using Word = unsigned int;

  SimpleAllocator() = default;

  SimpleAllocator(const SimpleAllocator&) = delete;
  SimpleAllocator& operator=(const SimpleAllocator&) = delete;

  SimpleAllocator(SimpleAllocator&&) = delete;
  SimpleAllocator& operator=(SimpleAllocator&&) = delete;

  // Before calling Malloc or free functions, the application must initialize the heap
  // by calling the Init function.
  bool Init();

  // An applicaton requests a block of size bytes of memory by calling Malloc.
  void* Malloc(size_t size);

  // Free a previously allocated block.
  void Free(void* bp);

 private:
  // Kernel-level helper.
  // Initialize the memory system model
  bool MemInit();

  // Simple model of the sbrk function.
  // Extends the heap by incr bytes and returns the start address of the new area.
  // In this model, the heap cannot be shrunk.
  void* MemSbrk(int incr);

 private:
  // App-level helper.
  // Extend the heap and create the initial free block.
  void* ExtendHeap(size_t nwords);

  // First fit search of implicit free list.
  // asize has been adjusted to include overhead and alignmnet reqs.
  void* FindFit(size_t asize);

  // Place the requested block at the beginning of the free block,
  // splitting only if the size of the remainder would equal or
  // exceed the minimus block size.
  // asize has been adjusted to include overhead and alignmnet reqs.
  void Place(void* bp, size_t asize);

  // Merge adjacent free blocks and return the block pointer
  // of the merged blocks.
  void* Coalesce(void* bp);

 private:
  // Points to first byte of heap
  Byte* mem_heap_ = nullptr;

  // Points to last byte of heap plus 1
  Byte* mem_brk_ = nullptr;

  // Max legal heap addr plus 1
  Byte* mem_max_addr_ = nullptr;

  // Points to first byte of heap
  Byte* mem_heap_prologue_ = nullptr;

  // The maximum block size
  static constexpr size_t kMaxHeap = (1 << 20);

 private:
  // Word and header/foot size(bytes)
  static constexpr size_t kWordSize = 4;

  // Double-word size(bytes)/Alignment size(minimus block size)
  static constexpr size_t kDoubleWordSize = 8;

  // Header/Footer 8 bytes.
  // Minimum payload 8 bytes.
  static constexpr size_t kMinimumRegularBlockSize = 16;

  // Extend heap by this amount(bytes)
  static constexpr size_t kChunkSize = (1 << 12);
};

}  // namespace csapp
