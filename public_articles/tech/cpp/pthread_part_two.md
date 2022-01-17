本文作为csapp 12.5(Synchronizing Threads with Semaphores)的读书笔记，主要介绍线程同步的相关问题。同时，也作为线程基础的第三部分讲义。

### Intro

- From a programmer’s perspective, one of the attractive aspects of threads is the
ease with which multiple threads can share the same program variables.
- However, this sharing can be tricky.
- In order to write correctly threaded programs, we must have a clear understanding of what we mean by sharing and how it works.

### Threads Memory Model

In computing, a memory model describes the interactions of threads through memory and their shared use of the data.

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

```cpp
// main thread
connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
Pthread_create(&tid, NULL, thread, &connfd);

// peer thread
void *thread(void *vargp) {
  int connfd = *((int *)vargp);
  ...
}
```

再看这段有问题的代码，connfd是一个stack var，但是peer thread还是可以拿到这个变量的访问权限。关键的问题就在于peer thread somehow manages to acquire a pointer to another thread’s stack.这其实会导致，不同线程之间共享变量。

这段代码的一种解决办法是，不要共享。
```cpp
// main thread
connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
Pthread_create(&tid, NULL, thread, reinterpret_cast<void*>connfd);

// peer thread
void *thread(void *vargp) {
  int connfd = reinterpret_cast<int>(vargp));
  ...
}
```
结论：T*和T，是两种不同的类型(不要特化指针类型，也只是一种普通类型)，变量里面放的东西都一样，只是取决于编译器怎么看这个东西。到底是一个操作数地址，还是一个操作数。

### Mapping Variables to Memory

Variables in threaded C programs are mapped to virtual memory according to
their storage classes:

- Global variables.
  - At run time, the read/write area of virtual memory contains exactly one instance of each global variable that can be referenced by any thread.
- Local automatic variables.
  - At run time, each thread’s stack contains its own instances of any local automatic variables.
  - This is true even if multiple threads execute the same thread routine.
- Local static variables.
  - As with global variables, the read/write area of virtual memory contains exactly one instance of each local static variable declared in a program.

### Shared Variables

- We say that a variable v is shared if and only if one of its instances is referenced by more than one thread.
- However, it is important to realize that local automatic variables such as msgs can also be shared.

```cpp
#include "csapp.h"
#define N 2
void *thread(void *vargp);

char **ptr; /* Global variable */

int main()
{
  int i;
  pthread_t tid;
  char *msgs[N] = {
  "Hello from foo",
  "Hello from bar"
  };

  ptr = msgs;
  for (i = 0; i < N; i++)
  Pthread_create(&tid, NULL, thread, (void *)i);
  Pthread_exit(NULL);
 }

 void *thread(void *vargp)
 {
  int myid = (int)vargp;
  static int cnt = 0;
  printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt);
  return NULL;
 }
```

| Variable instance | main thread | peer thread 0 | peer thread 1|
| :-----:| :----: | :----: |:----: |
| pctr | yes | yes | yes |
| cnt | no | yes | yes |
| i.m | yes | no | no |
| msgs.m | yes | yes | yes |
| myid.p0 | no | yes | no |
| myid.p1 | no | no | yes |

### 参考
[Memory model](https://en.cppreference.com/w/cpp/language/memory_model)
