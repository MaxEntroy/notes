### 引子

首先我们来看一段代码，这段demo是csapp 12.4给出的一个例子。考虑其中的变量在线程中可见性是怎样的？

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
  for (i = 0; i < N; i++) {
   Pthread_create(&tid, NULL, thread, (void *)i);
   Pthread_exit(NULL);
  }
 }

 void *thread(void *vargp)
 {
  int myid = (int)vargp;
  static int cnt = 0;
  printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt);
  return NULL;
 }
```

### 线程内存模型

要搞明白这个问题，需要参见threads memory model(Mapping Variables to Memory)的约定。

Variables in threaded C programs are mapped to virtual memory according to their storage classes:

- Global variables.
  - At run time, the read/write area of virtual memory contains exactly one instance of each global variable that can be referenced by any thread.
- Local automatic variables.
  - At run time, each thread’s stack contains its own instances of any local automatic variables.
  - This is true even if multiple threads execute the same thread routine.
- Local static variables.
  - As with global variables, the read/write area of virtual memory contains exactly one instance of each local static variable declared in a program.

通过上面的协议，可以给出变量在各个线程中的可见性

| Variable instance | main thread | peer thread 0 | peer thread 1|
| :-----:| :----: | :----: |:----: |
| pctr | yes | yes | yes |
| cnt | no | yes | yes |
| i.m | yes | no | no |
| msgs.m | yes | no | no |
| myid.p0 | no | yes | no |
| myid.p1 | no | no | yes |

通过i.m/msgs.m的可见性，可以发现pthread_create与fork在线程创建和进程创建中的差异，后者在fork点之前的变量，都会被拷贝到子进程中，而前者不会。对于线程而言，就是严格遵循了threads memory model的要求，对于main thread来说，fork点前后的stack var都不会被peer thread共享。

接下来，我们进一步考虑一个问题，既然local var在多线程之间不共享，那么向peer thread传递参数时，只传地址，不传值是否可以？

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
  for (i = 0; i < N; i++) {
   Pthread_create(&tid, NULL, thread, (void *)&i);  // pass a pointer to peer thread
   Pthread_exit(NULL);
  }
 }

 void *thread(void *vargp)
 {
  int myid = *((int)*vargp);  // dereference
  static int cnt = 0;
  printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt);
  return NULL;
 }
```

显然是不行的，因为这会引入data race. csapp给出如下解释。

- The memory model for the separate thread stacks is not as clean.
  - These stacks are contained in the stack area of the virtual address space and are usually accessed independently by their respective threads.
  - We say usually rather than always, because different thread stacks are not protected from other threads
  - So if a thread somehow manages to acquire a pointer to another thread’s stack, then it can read and write any part of that stack.

这里面核心的一点是：线程并没有能力保护自己的线程栈，所以一般而言，线程栈变量在线程之间无法共享，但是如果thread1获得了thread2的局部栈变量地址，那么此时有data race的可能。比如上面的代码，main thread在创建peer thread之后，到底谁先执行没有保证，如果此时main thread先执行，那么++i之后之后peer thread才会执行。而此时peer thread拿到的i值已经被更新过，不是当时希望打印的值，从而导致逻辑错误。所以，正确的办法应该是传值，避免共享。

当然，上面这段代码还有别的问题，csapp起初给出这段代码也只是为了当个引子，引出对于memory model的讨论。至于如何安全的访问共享变量，在之后的章节进行讨论。

### 一个类似的bug

写这篇文章的原因在于上周我犯了一个类似的错误，比较低级。说是低级是因为这些其实是最基础的东西，这些东西对于像我这样一个工作了4年的人来说，不应该再犯，但是我还是犯了。记下来，提醒自己，鞭策自己。

代码的具体逻辑是开协程进行文章分片打分，离线测试后一直coredump

```cpp
trpc::FiberLatch l(part_num);
for (int i = 0; i < part_num; ++i) {
  bool start_fiber = trpc::StartFiberDetached([&] {
    do {
      if (not GenerateReq(user_feats, item_feats, reqs[i])) {
        rets[i] = RetCode::kRequestError;
        TRPC_LOG_ERROR("partition " << i << " GenerateReq failed.");
        break;
      }
      rets[i] = SparsePredict(*reqs[i], rsps[i]);
      if (rets[i] != RetCode::kSuccess) {
        TRPC_LOG_ERROR("partition " << i << " SparsePredict failed.");
      }
    } while (0);
    l.CountDown();
  });
  if (TRPC_UNLIKELY(!start_fiber)) {
    l.CountDown();
    TRPC_LOG_ERROR("trpc::StartFiberDetached to execute task:" << i << "failed.");
  }
}
l.Wait();
```

紧接着gdb分析了coredump，fiber routine的入参i是1，这点和预期的不一致。因为这里打分的文章只有几十篇，分片后只有一个片，所以i应该是0。同时，因为i莫名其妙的变成了1，导致req/rsp全部越界，导致segmentation fault.问题到这里也算查到原因了，可是i为什么一直是1，这点起初我一直没搞明白。