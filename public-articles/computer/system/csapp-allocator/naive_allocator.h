#pragma once

namespace csapp {

class NaiveAllocator {
 public:
  NaiveAllocator() {

  }

 private:
  /// Points to first byte of heap
  char *mem_heap = nullptr;

  /// Points to last byte of heap plus 1
  char *mem_brk = nullptr;

  /// Max legal heap addr plus 1
  char *mem_max_addr = nullptr;

  /// The maximum block size
  static constexpr int kMaxHeap = 1 << 32;
};

}  // namespace csapp
