#include "naive_allocator.h"

#include <iostream>
#include <new>

namespace csapp {

bool NaiveAllocator::MemInit() {
  mem_heap_ = new (std::nothrow) Byte[kMaxHeap];
  if (!mem_heap_) {
    std::cerr << "Allocation returned nullptr.\n";
    return false;
  }
  mem_brk_ = mem_heap_;
  mem_max_addr_ = mem_brk_ + kMaxHeap;
  return true;
}

void* NaiveAllocator::MemSbrk(int incr) {
  auto* old_brk = mem_brk_;
  if (incr < 0 or mem_brk_ + incr > mem_max_addr_) {
    std::cerr << "Sbrk failed. Run out of memory.\n";
    return nullptr;
  }
  mem_brk_ += incr;
  return old_brk;
}

}  // namespace csapp
