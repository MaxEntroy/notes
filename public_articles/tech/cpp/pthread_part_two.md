本文作为csapp 12.4(Shared Variables in Threaded Programs)的读书笔记，主要介绍Threads Memory Model。同时，也作为线程基础的第二部分讲义。

### Intro

- From a programmer’s perspective, one of the attractive aspects of threads is the
ease with which multiple threads can share the same program variables.
- However, this sharing can be tricky.
- In order to write correctly threaded programs, we must have a clear understanding of what we mean by sharing and how it works.

### Threads Memory Model

- Each thread has its own separate thread context
  - which includes a thread ID, stack, stack pointer,program counter, condition codes, and general-purpose register values. 
- Each thread shares the rest of the process context with the other threads. 
  - This includes the entire user virtual address space, which consists of read-only text (code), read/write data, the heap, and any shared library code and data areas. 
  - The threads also share the same set of open files.
- Thus, registers are never shared, whereas virtual memory is always shared.

上面阐述了线程的基本内存模型，有一点需要特别注意，也是我们在上一小节遇到的问题：
- The memory model for the separate thread stacks is not as clean.
  - These stacks are contained in the stack area of the virtual address space and are usually
accessed independently by their respective threads. 
  - We say usually rather than always, because different thread stacks are not protected from other threads
  - So if a thread somehow manages to acquire a pointer to another thread’s stack, then it can read and write any part of that stack.

### Mapping Variables to Memory

### Shared Variables