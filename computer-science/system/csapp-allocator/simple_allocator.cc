#include "simple_allocator.h"

#include <iostream>
#include <new>

namespace csapp {

namespace {
// Manipulating the headers and footers in the free list can be troublesome
// because it demands extensive use of casting and pointer arithmetic.
// Thus, we find it helpful to define a small set of macros for accessing and
// traversing the free list

using Byte = SimpleAllocator::Byte;
using Word = SimpleAllocator::Word;

// Pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Read and write a word at address p
#define GET(p) (*reinterpret_cast<Word*>(p))
#define PUT(p, val) (GET(p) = (val))

// Read the size and allocated bits from address p
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

  // Given block ptr bp, compute address of its header and footer.
#define HEADER(bp) (reinterpret_cast<Byte*>(bp) - kWordSize)
#define FOOTER(bp) (reinterpret_cast<Byte*>(bp) + GET_SIZE(bp) - kDoubleWordSize)

// Given block ptr br, compute address of next and previous blocks.
#define NEXT_BLK(bp) (reinterpret_cast<Byte*>(bp) + GET_SIZE(HEADER(bp)))
#define PREV_BLK(bp) (reinterpret_cast<Byte*>(bp) + GET_SIZE((reinterpret_cast<Byte*>(bp) - kDoubleWordSize)))
}  // anonymous namespace

bool SimpleAllocator::Init() {
  // Initialize the memory system model
  bool is_init_succ = MemInit();
  if (!is_init_succ) {
    std::cerr << "MemInit failed.\n";
  }

  // Create the initial empty heap.
  mem_heap_prologue_ = static_cast<Byte*>(MemSbrk(4 * kWordSize));
  if (!mem_heap_prologue_) {
    return false;
  }

  PUT(mem_heap_prologue_, 0);  // Alignment padding.
  PUT(mem_heap_prologue_ + (1 * kWordSize), PACK(kDoubleWordSize, 1));  // Prologue header.
  PUT(mem_heap_prologue_ + (2 * kWordSize), PACK(kDoubleWordSize, 1));  // Prologue footer.
  PUT(mem_heap_prologue_ + (3 * kWordSize), PACK(0, 1));  // Epilogue header.

  // Regard the second prologue as the payload of the first prologue.
  // So, mem_heap_prologue_ points to a payload block.
  // We can apply it to macros like HEADER/FOOTER/NEXT_BLK/PREV_BLK.
  // What a wonderful design!
  mem_heap_prologue_ += (2 * kWordSize);

  // Extend the empty heap with a free block of CHUNKSIZE bytes.
  if (ExtendHeap(kChunkSize / kWordSize)) {
    return false;
  }

  return true;
}

void* SimpleAllocator::Malloc(size_t size) {
  // ignore spurious size
  if (!size)
    return nullptr;

  // Adjust block size to include overhead and alignment reqs.
  auto actual_size = size;
  if (size < kDoubleWordSize) {
    actual_size = kDoubleWordSize << 1;  // 8 bytes for head/footer, 8 bytes for payload.
  } else {
    actual_size = kDoubleWordSize * (kDoubleWordSize + size + (kDoubleWordSize - 1) / kDoubleWordSize);
  }

  // Search the free list for a fit.
  Byte* bp = nullptr;
  bp = reinterpret_cast<Byte*>(FindFit(actual_size));
  if (bp) {
    Place(bp, actual_size);
    return reinterpret_cast<void*>(bp);
  }

  // Not fit found. Get more memory and place the block.
  auto extend_size = std::max(actual_size, kChunkSize);
  bp = reinterpret_cast<Byte*>(ExtendHeap(extend_size / kWordSize));
  if (!bp) {
    std::cerr << "Not enough room.\n";
    return nullptr;
  }
  Place(bp, actual_size);

  return bp;
}

void SimpleAllocator::Free(void* bp) {
  // Free frees a block and uses boundary-tag coalescing to merge it
  // with any adjacent free blocks in constant time.
  size_t size = GET_SIZE(HEADER(bp));

  PUT(HEADER(bp), PACK(size, 0));
  PUT(FOOTER(bp), PACK(size, 0));

  Coalesce(bp);
}

void* SimpleAllocator::FindFit(size_t asize) {
  for (Byte* bp = NEXT_BLK(mem_heap_prologue_); GET_SIZE(HEADER(bp)); bp = NEXT_BLK(bp)) {
    // The first fit algorithm
    size_t alloc = GET_ALLOC(HEADER(bp));
    if (!alloc and GET_SIZE(HEADER(bp)) >= asize) {
      return bp;
    }
  }
  return nullptr;
}

void* SimpleAllocator::ExtendHeap(size_t nwords) {
  // Allocate an even number of words to maintain alignment.
  auto size = (nwords % 2) ? (nwords + 1) * kWordSize : nwords * kWordSize;
  auto* bp = static_cast<Byte*>(MemSbrk(size));
  if (!bp) {
    std::cerr << "Sbrk failed. Run out of memery.\n";
    return nullptr;
  }

  // Initialize free block header/footer and the epilogue header
  PUT(HEADER(bp), PACK(size, 0));  // free block header
  PUT(FOOTER(bp), PACK(size, 0));  // free block footer

  PUT(HEADER(NEXT_BLK(bp)), PACK(0, 1));  // New epilogue header
  return Coalesce(bp);
}

void SimpleAllocator::Place(void* bp, size_t asize) {
  // splitting only if the size of the remainder would equal or exceed
  // the minimum block size.
  size_t remainder = GET_SIZE(HEADER(bp)) - asize;
  if (remainder < kMinimumRegularBlockSize) { //
    // Allocate the whole block.
    PUT(HEADER(bp), PACK(GET_SIZE(HEADER(bp)), 1));
    PUT(FOOTER(bp), PACK(GET_SIZE(HEADER(bp)), 1));
  } else {  // Split the allocated block and the remainder.
    // Change the alloc of current block.
    PUT(HEADER(bp), PACK(asize, 1));
    PUT(FOOTER(bp), PACK(asize, 1));

    // Change the size of remainder block.
    // Splitting operation looks very smooth(just set some flags of data block.).
    PUT(HEADER(NEXT_BLK(bp)), PACK(remainder, 0));
    PUT(FOOTER(NEXT_BLK(bp)), PACK(remainder, 0));
  }
}

void* SimpleAllocator::Coalesce(void* bp) {
  size_t prev_alloc = GET_ALLOC(FOOTER(PREV_BLK(bp)));
  size_t next_alloc = GET_ALLOC(HEADER(NEXT_BLK(bp)));
  size_t size = GET_SIZE(HEADER(bp));

  if (!prev_alloc and !next_alloc) {
    return bp;
  } else if (prev_alloc and !next_alloc) {
    size += GET_SIZE(HEADER(NEXT_BLK(bp)));
    PUT(HEADER(bp), PACK(size, 0));
    PUT(FOOTER(bp), PACK(size, 0));
  } else if (!prev_alloc and next_alloc) {
    auto* prev_bp = PREV_BLK(bp);
    size += GET_SIZE(HEADER(prev_bp));
    // After the merge of 2 blocks, the footer of previous block will no longer exist,
    // and neither will the header of current block.
    PUT(HEADER(prev_bp), PACK(size, 0));
    PUT(FOOTER(bp), PACK(size, 0));
    bp = prev_bp;
  } else {
    auto* prev_bp = PREV_BLK(bp);
    auto* next_bp = NEXT_BLK(bp);
    size += GET_SIZE(HEADER(prev_bp));
    size += GET_SIZE(HEADER(next_bp));
    PUT(HEADER(prev_bp), PACK(size, 0));
    PUT(FOOTER(next_bp), PACK(size, 0));
    bp = prev_bp;
  }

  return bp;
}

bool SimpleAllocator::MemInit() {
  mem_heap_ = new (std::nothrow) Byte[kMaxHeap];
  if (!mem_heap_) {
    std::cerr << "Allocation returned nullptr.\n";
    return false;
  }
  mem_brk_ = mem_heap_;
  mem_max_addr_ = mem_brk_ + kMaxHeap;
  return true;
}

void* SimpleAllocator::MemSbrk(int incr) {
  // sbrk() returns the previous program break.
  auto* old_brk = mem_brk_;
  if (incr < 0 or mem_brk_ + incr > mem_max_addr_) {
    std::cerr << "Sbrk failed. Run out of memory.\n";
    return nullptr;
  }
  mem_brk_ += incr;
  return old_brk;
}

}  // namespace csapp
