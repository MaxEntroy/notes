#include "naive_allocator.h"

#include <iostream>
#include <new>

namespace csapp {

bool NaiveAllocator::InitInternal() {
  mem_heap = new(std::nothrow) char[kMaxHeap];
  if (not mem_heap) {
    std::cerr << "Allocation returned nullptr.\n";
    return false;
  }
  mem_brk = mem_heap;
  mem_max_addr = mem_brk + kMaxHeap;
  return true;
}

void* NaiveAllocator::Sbrk(int incr) {
  auto* old_sbrk = mem_brk;
  if (incr < 0 or mem_brk + incr > mem_max_addr) {
    std::cerr << "Sbrk failed. Ran out of memory.\n";
    return nullptr;
  }
  mem_brk += incr;
  return old_sbrk;
}

}  // namespace csapp
