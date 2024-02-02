#include "naive_allocator.h"

#include <iostream>
#include <new>

namespace csapp {

namespace {
// Manipulating the headers and footers in the free list can be troublesome
// because it demands extensive use of casting and pointer arithmetic.
// Thus, we find it helpful to define a small set of macros for accessing and
// traversing the free list

using Byte = NaiveAllocator::Byte;
using Word = NaiveAllocator::Word;

// Pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Read and write a word at address p
#define GET(p) (*reinterpret_cast<Word*>(p))
#define PUT(p, val) (GET(p) = (val))

// Read the size and allocated bits from address p
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

  // Given block ptr bp, compute address of its header and footer.
#define HEADER(bp) (reinterpret_cast<Btye*>(bp) - kWordSize)
#define FOOTER(bp) (reinterpret_cast<Btye*>(bp) + GET_SIZE(bp) - kDoubleWordSize)

// Given block ptr br, compute address of next and previous blocks.
#define NEXT_BLK(bp) (reinterpret_cast<Btye*>(bp) + GET_SIZE(HEADER(bp)))
#define PREV_BLK(bp) (reinterpret_cast<Btye*>(bp) + GET_SIZE((reinterpret_cast<Btye*>(bp) - DSIZE)))
}  // anonymous namespace

bool NaiveAllocator::Init() {
  // Create the initial empty heap.
  mem_heap_prologue_ = static_cast<Byte*>(MemSbrk(4 * kWordSize));
  if (!mem_heap_prologue_) {
    return false;
  }

  PUT(mem_heap_prologue_, 0);  // Alignment padding.
  PUT(mem_heap_prologue_ + (1 * kWordSize), PACK(kDoubleWordSize, 1));  // Prologue header.
  PUT(mem_heap_prologue_ + (2 * kWordSize), PACK(kDoubleWordSize, 1));  // Prologue header.
  PUT(mem_heap_prologue_ + (3 * kWordSize), PACK(0, 1));  // Epilogue header.

  mem_heap_prologue_ += (2 * kWordSize); // why?

  // Extend the empty heap with a free block of CHUNKSIZE bytes.
  if (ExtendHeap(kChunkSize / kWordSize)) {
    return false;
  }

  return true;
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

void* NaiveAllocator::ExtendHeap(size_t nwords) {
  // Allocate an even number of words to maintain alignment.
  auto size = (nwords % 2) ? (nwords + 1) * kWordSize : nwords * kWordSize;
  auto* bp = static_cast<Byte*>(MemSbrk(size));
  if (!bp) {
    std::cerr << "Sbrk failed. Run out of memery.\n";
    return nullptr;
  }

  // Initialize free block header/footer and the epilogue header
  return nullptr;
}


}  // namespace csapp
