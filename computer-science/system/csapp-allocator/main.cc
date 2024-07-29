#include <iostream>
#include "simple_allocator.h"

using namespace csapp;

int main(void) {
  SimpleAllocator alloc;
  alloc.Init();

  int* pval = static_cast<int*>(alloc.Malloc(4));
  *pval = 32;
  std::cout << *pval << std::endl;

  alloc.Free(pval);

  return 0;
}
