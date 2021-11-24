本文借助Posix threads，介绍thread的一些基本概念

### 基本概念

### Posix Threads

>Posix threads (Pthreads) is a standard interface for manipulating threads from C
programs. It was adopted in 1995 and is available on all Linux systems. Pthreads
defines about 60 functions that allow programs to create, kill, and reap threads,
to share data safely with peer threads, and to notify peers about changes in the
system state.

简言之，linux下的线程库。紧接着，我们来看一下常用的线程函数

#### Creating Threads

- Threads create other threads by calling the pthread_create function.
- When pthread_create returns, argument tid contains the ID of the newly
created thread. The new thread can determine its own thread ID by calling the
pthread_self function.

```cpp
#include <pthread.h>
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
// Returns: 0 if OK, nonzero on error

pthread_t pthread_self(void);
// Returns: thread ID of caller
```

#### Terminating Threads

A thread terminates in one of the following ways:
- The thread terminates implicitly when its top-level thread routine returns.
- The thread terminates explicitly by calling the pthread_exit function. If
the main thread calls pthread_exit, it waits for all other peer threads to
terminate and then terminates the main thread and the entire process with
a return value of thread_return.

```cpp
#include <pthread.h>
void pthread_exit(void *thread_return);
// Never returns

#include <pthread.h>
int pthread_cancel(pthread_t tid);
// Returns: 0 if OK, nonzero on error
```

- Some peer thread calls the Linux exit function, which terminates the process
and all threads associated with the process.
- Another peer thread terminates the current thread by calling the pthread_
cancel function with the ID of the current thread.

#### Reaping Terminated Threads
这里要注意区别Terminating和Reaping的概念。Reaping的概念和在介绍进程时是一致的，主要是回收进程/线程资源

- Threads wait for other threads to terminate by calling the pthread_join function. The pthread_join function 
  - blocks until thread tid terminates, 
  - assigns the generic (void *) pointer returned by the thread routine to the location pointed to by thread_return, 
  - and then reaps any memory resources held by the terminated thread.

这里要特别注意，一定要理解清楚pthread_join到底做了几件事情，因为大家容易把pthread_join和barriers的语义搞混。其实二者并不一样

根据apue当中给出的barries定义，我们可以知道，pthread_join有barriers的语义，但是还有其余的功能。因为后者显然没有assigns/reaps的作用。
>Barriers are a synchronization mechanism that can be used to coordinate multiple
threads working in parallel. A barrier allows each thread to wait until all cooperating
threads have reached the same point, and then continue executing from there. We’ve
already seen one form of barrier—the pthread_join function acts as a barrier to
allow one thread to wait until another thread exits.

```cpp
#include <pthread.h>
int pthread_join(pthread_t tid, void **thread_return);

// Returns: 0 if OK, nonzero
```

#### Detaching Threads

A Thread can run in two modes i.e. Joinable Mode, Detached Mode

- Joinable Thread & pthread_join()
  - By default a thread runs in joinable mode. Joinable thread will not release any resource even after the end of thread function, until some other thread calls pthread_join() with its ID.
  - pthread_join() is a blocking call, it will block the calling thread until the other thread ends.
- Detached Thread & pthread_detach()
  - A Detached thread automatically releases it allocated resources on exit. No other thread needs to join it.
  - But by default all threads are joinable, so to make a thread detached we need to call pthread_detach() with thread id i.e.

```cpp
#include <pthread.h>
int pthread_detach(pthread_t tid);

// Returns: 0 if OK, nonzero on error
```

By default, threads are created joinable. In order to avoid memory leaks, each
joinable thread should be either explicitly reaped by another thread or detached
by a call to the pthread_detach function.

Q:When to use detached threads?
>Although some of our examples will use joinable threads, there are good reasons
to use detached threads in real programs. For example, a high-performance
Web server might create a new peer thread each time it receives a connection request
from a Web browser. Since each connection is handled independently by a
separate thread, it is unnecessary—and indeed undesirable—for the server to explicitly
wait for each peer thread to terminate. In this case, each peer thread should
detach itself before it begins processing the request so that its memory resources
can be reclaimed after it terminates.

#### Initializing Threads

The pthread_once function allows you to initialize the state associated with a
thread routine.

```cpp
#include <pthread.h>
pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));

// Always returns 0
```

- The once_control variable is a global or static variable that is always initialized
to PTHREAD_ONCE_INIT.
- The first time you call pthread_once with an argument
of once_control, it invokes init_routine, which is a function with no
input arguments that returns nothing
- Subsequent calls to pthread_once with the
same once_control variable do nothing.

The pthread_once function is useful whenever you need to dynamically initialize global variables that are shared by
multiple threads.

简言之，多线程初始化的问题，需要依靠pthread_once解决。

### 参考
[POSIX : Detached vs Joinable threads | pthread_join() & pthread_detach() examples](https://thispointer.com/posix-detached-vs-joinable-threads-pthread_join-pthread_detach-examples/)