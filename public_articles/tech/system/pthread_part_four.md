本文作为csapp 12其余小节的读书笔记，主要介绍并发相关的剩余问题。同时，也作为线程基础的第四部分讲义。

### Using Threads for Parallelism

The set of all programs can be partitioned into the disjoint sets of sequential and concurrent programs.
- A sequential program is written as a single logical flow.
- A concurrent program is written as multiple concurrent flows.(logical control flows are concurrent if they overlap in time.)
- A parallel program is a concurrent program running on multiple processors

多线程程序编写起来有一定门槛，下面我们通过一个经典的例子(sum of [0, n-1])，来了解这其中的一些细节

```cpp
// For example, consider how we might sum the sequence of integers 0, . . . , n − 1 in parallel.
 
/* Thread routine for psum-mutex.c */
void *sum_mutex(void *vargp)
{
  long myid = *((long *)vargp); /* Extract the thread ID */
  long start = myid * nelems_per_thread; /* Start element index */
  long end = start + nelems_per_thread; /* End element index */
  long i;

  for (i = start; i < end; i++) {
   P(&mutex);
   gsum += i;
   V(&mutex);
  }
  return NULL;
}
```

| version\number of threads| 1 | 2 |4 | 8| 16|
| :-----:| :----: | :----: |:----: |:----: |:----: |
| psum-mutex | 68 | 432 | 719 | 552 | 559 |

- Not only is the program extremely slow when it runs sequentially as a single
thread, it is nearly an order of magnitude slower when it runs in parallel as
multiple threads.
- And the performance gets worse as we add more cores.

**The reason for this poor performance is that the synchronization operations (P and V ) are very expensive relative to the cost of a single memory update.**

This highlights an important lesson about parallel programming: **Synchronization overhead is expensive and should be avoided if possible. If it cannot be avoided, the overhead should be amortized by as much useful computation as possible.**

```cpp
/* Thread routine for psum-array.c */
void *sum_array(void *vargp)
{
  long myid = *((long *)vargp); /* Extract the thread ID */
  long start = myid * nelems_per_thread; /* Start element index */
  long end = start + nelems_per_thread; /* End element index */
  long i;
  
  for (i = start; i < end; i++) {
   psum[myid] += i;
  }
  return NULL;
 }
```

Since we are careful to give each peer thread a unique memory location to update, it is not necessary to protect these updates with mutexes.The only
necessary synchronization is that the main thread must wait for all of the children to finish.

| version\number of threads| 1 | 2 |4 | 8| 16|
| :-----:| :----: | :----: |:----: |:----: |:----: |
| psum-mutex | 68 | 432 | 719 | 552 | 559 |
| psum-array | 7.26 | 3.64 | 1.91 | 1.85 | 1.84 |

```cpp
/* Thread routine for psum-local.c */
void *sum_local(void *vargp)
{
  long myid = *((long *)vargp); /* Extract the thread ID */
  long start = myid * nelems_per_thread; /* Start element index */
  long end = start + nelems_per_thread; /* End element index */
  long i, sum = 0;
  
  for (i = start; i < end; i++) {
    sum += i;
  }
  psum[myid] = sum;
  return NULL;
}
```

In Chapter 5, we learned how to use local variables to eliminate unnecessary
memory references. ```sum_local``` shows how we can apply this principle by having
each peer thread accumulate its partial sum into a local variable rather than
a global variable. When we run psum-local on our four-core machine, we get
another order-of-magnitude decrease in running time:

| version\number of threads| 1 | 2 |4 | 8| 16|
| :-----:| :----: | :----: |:----: |:----: |:----: |
| psum-mutex | 68 | 432 | 719 | 552 | 559 |
| psum-array | 7.26 | 3.64 | 1.91 | 1.85 | 1.84 |
| psum-local | 1.06| 0.54| 0.28 | 0.29| 0.30 |

### Other Concurrency Issues

You probably noticed that life got much more complicated once we were asked
to synchronize accesses to shared data.<br>
So far, we have looked at techniques for mutual exclusion and producer-consumer synchronization, but this is only the tip
of the iceberg.<br>
Synchronization is a fundamentally difficult problem that raises
issues that simply do not arise in ordinary sequential programs.<br>

This section is a survey (by no means complete) of some of the issues you need to be aware of when you write concurrent programs. 

#### Thread Safety

关于thread safe的定义，我多说一点，这里没有采纳csapp给出的定义，也没有采用chenshuo在muduo当中给出的引用定义，而是直接使用了wikipedia给出的定义，原因在于，下面的定义不针对具体的数据结构，也不针对具体的函数。比较通用。

>Implementation is guaranteed to be free of race conditions when accessed by multiple threads simultaneously.

具体来说，当我们说一个函数不是线程安全时，即多线程的调度执行时，无法保证程序执行的正确性。同理，当我们说一个数据结构不是线程安全时，即多线程访问时，无法保证多线程代码执行的正确性。
所以，如果一个函数/数据结构我们说它是线程安全的，一定是这个函数/数据结构，做了额外的操作消除了race condition.

Below we discuss two classes of approaches for avoiding race conditions to achieve thread-safety.

- The first class of approaches focuses on avoiding shared state and includes:
  - Re-entrancy
    - they do not reference any shared data when they are called by multiple threads
    - Reentrant functions are typically more efficient than non-reentrant threadsafe functions because they require no synchronization operations.
  - Thread-local storage
  - Immutable objects
- The second class of approaches are synchronization-related, and are used in situations where shared state cannot be avoided:
  - Mutual exclusion
  - Atomic operations

#### Using Existing Library Functions in Threaded Programs

Most Linux functions, including the functions defined in the standard C library
(such as malloc, free, realloc, printf, and scanf), are thread-safe, with only
a few exceptions. 

| Thread-unsafe function | 
| :-----:|
| rand| 
| strtok| 
| asctime |
| ctime | 
| gethostbyaddr |
| gethostbyname |
| inet_ntoa |
| localtime |

#### Deadlock

In concurrent computing, a deadlock is a state in which each member of a group waits for another member, including itself, to take action, such as sending a message or more commonly releasing a lock.(P1 requires additional resource R1 and is in possession of resource R2, P2 requires additional resource R2 and is in possession of R1; neither process can continue)

```cpp
thread1: P(s)P(t)V(s)V(t)
thread2: P(t)P(s)V(t)V(s)
```

Mutex lock ordering rule: **Given a total ordering of all mutexes, a program is deadlock-free if each thread acquires its mutexes in order and releases them in reverse order**.

```cpp
thread1: P(s)P(t)V(t)V(s)
thread2: P(s)P(t)V(t)V(s)
```

#### Summary

Aconcurrent program consists of a collection of logical flows that overlap in time.

- Processes are scheduled automatically by the kernel, and because of their
separate virtual address spaces, they require explicit IPC mechanisms in order
to share data.
- Event-driven programs create their own concurrent logical flows,
which are modeled as state machines, and use I/O multiplexing to explicitly schedule
the flows. Because the program runs in a single process, sharing data between
flows is fast and easy.
- Threads are a hybrid of these approaches. Like flows based
on processes, threads are scheduled automatically by the kernel. Like flows based
on I/O multiplexing, threads run in the context of a single process, and thus can
share data quickly and easily.

### References

[Thread safety](https://en.wikipedia.org/wiki/Thread_safety)<br>
[Deadlock](https://en.wikipedia.org/wiki/Deadlock)
