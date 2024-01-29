#include "naive_allocator.h"

#include <iostream>
#include <new>

namespace csapp {

namespace {
// Manipulating the headers and footers in the free list can be troublesome
// because it demands extensive use of casting and pointer arithmetic.
// Thus, we find it helpful to define a small set of macros for accessing and
// traversing the free list

// Pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Deference address p based on specific type
#define CAST(p, Type) (static_cast<Type*>(p))
#define DEFER(p, Type) (*CAST(p, Type))

// Read and write a word at address p
#define GET(p) (DEFER(p, NaiveAllocator::Word))
#define PUT(p, val) (DEFER(p, NaiveAllocator::Word) = (val))

// Read the size and allocated bits from address p
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

  // Given block ptr bp, compute address of its header and footer.
#define HEADER(bp) (CAST(bp, NaiveAllocator::Byte) - NaiveAllocator::kWordSize)
#define FOOTER(bp) (CAST(bp, NaiveAllocator::Byte) + GET_SIZE(HEADER(bp)) - NaiveAllocator::kDoubleWordSize)

// Given block ptr br, compute address of next and previous blocks.
#define NEXT_BLK(bp) (CAST(bp, NaiveAllocator::Byte) + GET_SIZE(HEADER(bp)))
#define PREV_BLK(bp)                                                           \
  (CAST(bp, NaiveAllocator::Byte) -                                            \
   GET_SIZE(                                                                   \
       (CAST(bp, NaiveAllocator::Byte) - NaiveAllocator::kDoubleWordSize)))
}

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
  // sbrk() returns the previous program break.
  auto* old_brk = mem_brk_;
  if (incr < 0 or mem_brk_ + incr > mem_max_addr_) {
    std::cerr << "Sbrk failed. Run out of memory.\n";
    return nullptr;
  }
  mem_brk_ += incr;
  return old_brk;
}

bool NaiveAllocator::Init() {
  // Create the initial empty heap.
  if (!(mem_heap_prologue_ = MemSbrk(2 * kDoubleWordSize))) {
    return false;
  }

  PUT(mem_heap_prologue_, 0);
  PUT(mem_heap_prologue_ + (1 * kWordSize), PACK(kDoubleWordSize, 1));
  PUT(mem_heap_prologue_ + (2 * kWordSize), PACK(kDoubleWordSize, 1));
  return true;
}

}  // namespace csapp
