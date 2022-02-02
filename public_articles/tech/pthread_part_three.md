本文作为csapp 12.5(Synchronizing Threads with Semaphores)的读书笔记，主要介绍线程同步的相关问题。同时，也作为线程基础的第三部分讲义。

### Intro

首先来谈以下同步的概念，因为这个概念充斥在各种场景中(进程/线程同步，同步IO/异步IO，数据同步)，容易对这个概念本身理解的不够透彻。

下面我们引入wiki的对于同步的理解
>Synchronization is the coordination of events to operate a system in unison.

可以看出，同步通过对于系统中事件的协调，来达到时空的一致性。

不过，我倾向于，对于不同的场景，做不同特化的描述。这样，面试官好明白。

- Process synchronization refers to the idea that multiple processes are to join up or handshake at a certain point, in order to reach an agreement or commit to a certain sequence of action. 
- Data synchronization refers to the idea of keeping multiple copies of a dataset in coherence with one another, or to maintain data integrity. Process synchronization primitives are commonly used to implement data synchronization.
- Synchronous I/O. A simple approach to I/O would be to start the access and then wait for it to complete. But such an approach (called synchronous I/O, or blocking I/O) would block the progress of a program while the communication is in progress, leaving system resources idle. When a program makes many I/O operations (such as a program mainly or largely dependent on user input), this means that the processor can spend almost all of its time idle waiting for I/O operations to complete.
- Asynchronous I/O. Alternatively, it is possible to start the communication and then perform processing that does not require that the I/O be completed. This approach is called asynchronous input/output. Any task that depends on the I/O having completed (this includes both using the input values and critical operations that claim to assure that a write operation has been completed) still needs to wait for the I/O operation to complete, and thus is still blocked, but other processing that does not have a dependency on the I/O operation can continue.

下面我们回到本章节的主题，详细介绍一下进程/线程同步的概念

#### The need for synchronization

The need for synchronization does not arise merely in multi-processor systems but for any kind of concurrent processes; even in single processor systems. Mentioned below are some of the main needs for synchronization:two or more works at a same time

- Forks and Joins: When a job arrives at a fork point, it is split into N sub-jobs which are then serviced by n tasks. After being serviced, each sub-job waits until all other sub-jobs are done processing. Then, they are joined again and leave the system. Thus, parallel programming requires synchronization as all the parallel processes wait for several other processes to occur.
- Producer-Consumer: In a producer-consumer relationship, the consumer process is dependent on the producer process till the necessary data has been produced.
- Exclusive use resources: When multiple processes are dependent on a resource and they need to access it at the same time, the operating system needs to ensure that only one processor accesses it at a given point in time. This reduces concurrency.

#### Thread or process synchronization

- Thread synchronization is defined as a mechanism which ensures that two or more concurrent processes or threads do not simultaneously execute some particular program segment known as critical section.Processes' access to critical section is controlled by using synchronization techniques.
- When one thread starts executing the critical section (serialized segment of the program) the other thread should wait until the first thread finishes. If proper synchronization techniques are not applied, it may cause a race condition where the values of variables may be unpredictable and vary depending on the timings of context switches of the processes or threads.

这里说下我的理解，同步机制是指，在并发环境下，保证程序并发执行正确性的手段。这个手段具体来说，就是协调进程/线程的执行顺序，来保证时空的一致性(回应本文一开始提出的同步概念)

这里我们多探讨两组概念：
- critical section: program where the shared resource is accessed, **it cannot be executed by more than one process at a time**
- race condition: the system's substantive behavior is dependent on the sequence or timing of other uncontrollable events.

竞争条件这里我多说一点：
>它旨在描述一个系统或者进程的输出依赖于不受控制的事件出现顺序或者出现时机。此词源自于两个信号试着彼此竞争，来影响谁先输出。
举例来说，如果计算机中的两个进程同时试图修改一个共享内存的内容，在没有并发控制的情况下，最后的结果依赖于两个进程的执行顺序与时机。而且如果发生了并发访问冲突，则最后的结果是不正确的。

也就是说，竞争条件将不期望的随机性引入了系统，导致程序的正确性受到影响。

我们接下来看看date race

The Thread Analyzer detects data-races that occur during the execution of a multi-threaded process. A data race occurs when:
- two or more threads in a single process access the same memory location concurrently, and
- at least one of the accesses is for writing, and
- the threads are not using any exclusive locks to control their accesses to that memory.

When these three conditions hold, the order of accesses is non-deterministic, and the computation may give different results from run to run depending on that order. Some data-races may be benign (for example, when the memory access is used for a busy-wait), but many data-races are bugs in the program.

那么，我们好奇的是，data race和race condition的关系，直接给出stackoverflow的结论
>No, they are not the same thing. They are not a subset of one another. They are also neither the necessary, nor the sufficient condition for one another.
Many race conditions can be caused by data races, but this is not necessary.

我们可以看下面这段代码
```cpp
#include <atomic>
std::atomic<int> my_account = 0;
std::atomic<int> your_account = 100;

bool unsafe_transfer(std::atomic<int>& src, std::atomic<int>& dst, int m)
{
  if (m <= src) {
    src -= m;
    dst += m;
    return true;
  } else {
    return false;
  }
}

unsafe_transfer(your_account, my_account, 50);//[A]
unsafe_transfer(your_account, my_account, 80);//[B]
```

显然，上面的代码没有data race，因为my_account/your_account有对应的同步机制进行保护。但是，上面的代码会出现race condition。因为整个transfer其实是原子语义，否则总共就100块钱，上面代码的
可能结果有：
- thread a执行完成，thread b开始执行，结果正确
- thread b执行完成，thread a开始执行，结果正确。
- thread a/thread b同时执行到判断逻辑，都满足要求，结果错误。

```cpp
#include <mutex>
int my_account = 0;
int your_account = 100; 
std::mutex txn_guard;

bool safe_transfer(int& src, int& dst, int m)
{
  // critical section
  std::lock_guard<std::mutex> lk(txn_guard);
  if (m <= src) {
    src -= m;
    dst += m;
    return true;
  } else {
    return false;
  }
}  

safe_transfer(your_account, my_account, 50);  // [A]
safe_transfer(your_account, my_account, 80);  // [B]
```

其实不管是race condition还是data race，我们可以发现race其实会引入non-deterministic，导致程序的正确性受到影响

#### RMO operations

下面我们来详细分析一个例子

```cpp
void *thread(void *vargp); /* Thread routine prototype */

/* Global shared variable */
volatile long cnt = 0; /* Counter */

int main(int argc, char **argv)
{
  long niters;
  pthread_t tid1, tid2;
  
  /* Check input argument */
  if (argc != 2) {
    printf("usage: %s <niters>\n", argv[0]);
    exit(0);
  }
  niters = atoi(argv[1]);
  
  /* Create threads and wait for them to finish */
  Pthread_create(&tid1, NULL, thread, &niters);
  Pthread_create(&tid2, NULL, thread, &niters);
  Pthread_join(tid1, NULL);
  Pthread_join(tid2, NULL);
  
  /* Check result */
  if (cnt != (2 * niters))
    printf("BOOM! cnt=%ld\n", cnt);
  else
    printf("OK cnt=%ld\n", cnt);
  exit(0);
}

/* Thread routine */
void *thread(void *vargp)
{
  long i, niters = *((long *)vargp);
  
  for (i = 0; i < niters; i++)
    cnt++;
  
  return NULL;
}
```

To understand the problem clearly, we need to study the assembly code for the counter loop (lines 40–41),
- Hi : The block of instructions at the head of the loop
- Li : The instruction that loads the shared variable cnt into the accumulator register %rdxi , where %rdxi denotes the value of register %rdx in thread i
- Ui : The instruction that updates (increments) %rdxi
- Si : The instruction that stores the updated value of %rdxi back to the shared variable cnt
- Ti : The block of instructions at the tail of the loop

1. When the two peer threads in badcnt.c run concurrently on a uniprocessor, the machine instructions are completed one after the other in some order. 
2. Thus, each concurrent execution defines some total ordering (or interleaving) of the instructions in the two threads. 
3. Unfortunately, some of these orderings will produce correct results, but others will not.

**Here is the crucial point: In general, there is no way for you to predict whether the operating system will choose a correct ordering for your threads.**

上面这句话讲到根本了，kernel也无法保证一个固定的instruction order，这有可能会导致机器指令的交替执行，从而导致错误的结果。当然，从结论中，我们可以反推一些前提条件
- assembly code是指令执行的最小单元(这个只是我的反推，没有校验)
- 即使都是cpu操作，也会发生汇编指令级别的乱序(是否有可能cpu执行时，不进行调度，只有io才进行调度)

### Semaphores

A semaphore, s, is a **global variable** with a nonnegative integer value that can only be manipulated by two special operations, called P and V
- P(s):
  - If s is nonzero, then P decrements s and returns immediately. 
  - If s iszero, then suspend the thread until s becomes nonzero and the thread is restarted by a V operation. 
  - After restarting, the P operation decrements s and returns control to the caller.
- V(s):
  - The V operation increments s by 1.
  - If there are any threads blocked at a P operation waiting for s to become nonzero, then the V operation restarts exactly one of these threads,
  - which then completes its P operation by decrementing s.

这里有一点要注意，P操作，无论如何都要减一，V操作，无论如何都要加1.关于PV操作，还有一些需要注意的地方：

- The test and decrement operations in P occur indivisibly
- Notice that the definition of V does not define the order in which waiting threads are restarted.Thus, when several threads are waiting at a semaphore, you cannot predict which one will be restarted as a result of the V .

**Regardless, the message remains the same: always synchronize accesses to your shared variables, regardless if you’re running on a uniprocessor or a multiprocessor**

#### Using Semaphores for Mutual Exclusion

Semaphores provide a convenient way to ensure mutually exclusive access to shared variables. The basic idea is to associate a semaphore s, initially 1, with each shared variable and then surround the corresponding critical section with P(s) and V (s) operations.

- Binary semaphores whose purpose is to provide mutual exclusion are often called mutexes.
- Performing a P operation on a mutex is called locking the mutex.
- Similarly, performing the V operation is called unlocking the mutex.
- A thread that has locked but not yet unlocked a mutex is said to be holding the mutex

```cpp
void *thread(void *vargp); /* Thread routine prototype */

/* Global shared variable */
volatile long cnt = 0; /* Counter */

#define P(s) sem_wait(&s)
#define V(s) sem_post(&s)

sem_t mtx;

int main(int argc, char **argv)
{
  long niters;
  pthread_t tid1, tid2;
  
  /* Check input argument */
  if (argc != 2) {
    printf("usage: %s <niters>\n", argv[0]);
    exit(0);
  }
  niters = atoi(argv[1]);

  sem_init(&mtx, 0, 1);

  /* Create threads and wait for them to finish */
  Pthread_create(&tid1, NULL, thread, &niters);
  Pthread_create(&tid2, NULL, thread, &niters);
  Pthread_join(tid1, NULL);
  Pthread_join(tid2, NULL);
  
  /* Check result */
  if (cnt != (2 * niters))
    printf("BOOM! cnt=%ld\n", cnt);
  else
    printf("OK cnt=%ld\n", cnt);
  exit(0);
}

/* Thread routine */
void *thread(void *vargp)
{
  long i, niters = *((long *)vargp);
  
  for (i = 0; i < niters; i++) {
    P(mtx);
    cnt++;
    V(mtx);
  }
  return NULL;
}
```

#### Using Semaphores to Schedule Shared Resources

Another important use of semaphores, besides providing mutual exclusion, is to
schedule accesses to shared resources. In this scenario, a thread uses a semaphore operation to **notify** another thread that **some condition in the program state has become true.**

##### Producer-Consumer Problem

- Since inserting and removing items involves updating shared variables, we must guarantee mutually exclusive access to the buffer.
- But guaranteeing mutual exclusion is not sufficient. We also need to schedule accesses to the buffer.
  - If the buffer is full (there are no empty slots), then the producer must wait until a slot becomes available.
  - Similarly, if the buffer is empty (there are no available items), then the consumer must wait until an item becomes available.

```cpp
typedef struct {
  int *buf; /* Buffer array */
  int n; /* Maximum number of slots */
  int front; /* buf[(front+1)%n] is first item */
  int rear; /* buf[rear%n] is last item */
  sem_t mutex; /* Protects accesses to buf */
  sem_t slots; /* Counts available slots */
  sem_t items; /* Counts available items */
 } sbuf_t;

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n)
{
  sp->buf = Calloc(n, sizeof(int));
  sp->n = n; /* Buffer holds max of n items */
  sp->front = sp->rear = 0; /* Empty buffer iff front == rear */
  Sem_init(&sp->mutex, 0, 1); /* Binary semaphore for locking */
  Sem_init(&sp->slots, 0, n); /* Initially, buf has n empty slots */
  Sem_init(&sp->items, 0, 0); /* Initially, buf has zero data items */
}

/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp)
{
  Free(sp->buf);
}

/* Insert item onto the rear of shared buffer sp */
void sbuf_insert(sbuf_t *sp, int item)
{
  P(&sp->slots); /* Wait for available slot */
  P(&sp->mutex); /* Lock the buffer */
  sp->buf[(++sp->rear)%(sp->n)] = item; /* Insert the item */
  V(&sp->mutex); /* Unlock the buffer */
  V(&sp->items); /* Announce available item */
}

/* Remove and return the first item from buffer sp */
int sbuf_remove(sbuf_t *sp)
{
  int item;
  P(&sp->items); /* Wait for available item */
  P(&sp->mutex); /* Lock the buffer */
  item = sp->buf[(++sp->front)%(sp->n)]; /* Remove the item */
  V(&sp->mutex); /* Unlock the buffer */
  V(&sp->slots); /* Announce available slot */
  return item;
}
```

- 这里核心的理解是，用信号量来表达共享资源的个数，不管是用来同步互斥，还是同步调度。
- 不过，我个人觉得，可能条件变量的语义更清楚一点。当然，信号量的语义也很清楚，但是如果我们换个问题，比如某个条件发生时，线程被阻塞。某个条件发生时，线程被唤醒。此时如果用信号量就不是很直观。因为，某个条件是否能用信号量的语义来表达，这个不一定。上面的条件刚好是资源数，所以信号量的表达没问题。

#### Readers–writers problem

这个问题也可以用信号量建模，本质是因为读写的资源，一般是memory arena(a shared variable)，所以我们可以用一个信号量来表示资源的个数。

First readers–writers problem
>Suppose we have a shared memory area (critical section) with the basic constraints detailed above. It is possible to protect the shared data behind a mutual exclusion mutex, in which case no two threads can access the data at the same time. However, this solution is sub-optimal, because it is possible that a reader R1 might have the lock, and then another reader R2 requests access. It would be foolish for R2 to wait until R1 was done before starting its own read operation; instead, R2 should be allowed to read the resource alongside R1 because reads don't modify data, so concurrent reads are safe. This is the motivation for the first readers–writers problem, in which the constraint is added that no reader shall be kept waiting if the share is currently opened for reading. This is also called readers-preference, with its solution:

```cpp
semaphore resource=1;
semaphore rmutex=1;
readcount=0;

/*
   resource.P() is equivalent to wait(resource)
   resource.V() is equivalent to signal(resource)
   rmutex.P() is equivalent to wait(rmutex)
   rmutex.V() is equivalent to signal(rmutex)
*/

writer() {
    resource.P();          //Lock the shared file for a writer

    <CRITICAL Section>
    // Writing is done

    <EXIT Section>
    resource.V();          //Release the shared file for use by other readers. Writers are allowed if there are no readers requesting it.
}

reader() {
    rmutex.P();           //Ensure that no other reader can execute the <Entry> section while you are in it
    <CRITICAL Section>
    readcount++;          //Indicate that you are a reader trying to enter the Critical Section
    if (readcount == 1)   //Checks if you are the first reader trying to enter CS
        resource.P();     //If you are the first reader, lock the resource from writers. Resource stays reserved for subsequent readers
    <EXIT CRITICAL Section>
    rmutex.V();           //Release

    // Do the Reading

    rmutex.P();           //Ensure that no other reader can execute the <Exit> section while you are in it
    <CRITICAL Section>
    readcount--;          //Indicate that you no longer need the shared resource. One fewer reader
    if (readcount == 0)   //Checks if you are the last (only) reader who is reading the shared file
        resource.V();     //If you are last reader, then you can unlock the resource. This makes it available to writers.
    <EXIT CRITICAL Section>
    rmutex.V();           //Release
}
```

Second readers–writers problem
>The first solution is suboptimal, because it is possible that a reader R1 might have the lock, a writer W be waiting for the lock, and then a reader R2 requests access. It would be unfair for R2 to jump in immediately, ahead of W; if that happened often enough, W would starve. Instead, W should start as soon as possible. This is the motivation for the second readers–writers problem, in which the constraint is added that no writer, once added to the queue, shall be kept waiting longer than absolutely necessary. This is also called writers-preference.

```cpp
int readcount, writecount;                   //(initial value = 0)
semaphore rmutex, wmutex, readTry, resource; //(initial value = 1)

//READER
reader() {
<ENTRY Section>
  readTry.P();                 //Indicate a reader is trying to enter
  rmutex.P();                  //lock entry section to avoid race condition with other readers
  readcount++;                 //report yourself as a reader
  if (readcount == 1)          //checks if you are first reader
    resource.P();              //if you are first reader, lock  the resource
  rmutex.V();                  //release entry section for other readers
  readTry.V();                 //indicate you are done trying to access the resource

<CRITICAL Section>
//reading is performed

<EXIT Section>
  rmutex.P();                  //reserve exit section - avoids race condition with readers
  readcount--;                 //indicate you're leaving
  if (readcount == 0)          //checks if you are last reader leaving
    resource.V();              //if last, you must release the locked resource
  rmutex.V();                  //release exit section for other readers
}

//WRITER
writer() {
<ENTRY Section>
  wmutex.P();                  //reserve entry section for writers - avoids race conditions
  writecount++;                //report yourself as a writer entering
  if (writecount == 1)         //checks if you're first writer
    readTry.P();               //if you're first, then you must lock the readers out. Prevent them from trying to enter CS
  wmutex.V();                  //release entry section
  resource.P();                //reserve the resource for yourself - prevents other writers from simultaneously editing the shared resource
<CRITICAL Section>
  //writing is performed
  resource.V();                //release file

<EXIT Section>
  wmutex.P();                  //reserve exit section
  writecount--;                //indicate you're leaving
  if (writecount == 0)         //checks if you're the last writer
    readTry.V();               //if you're last writer, you must unlock the readers. Allows them to try enter CS for reading
  wmutex.V();                  //release exit section
}
```

Third readers–writers problem
>In fact, the solutions implied by both problem statements can result in starvation — the first one may starve writers in the queue, and the second one may starve readers. Therefore, the third readers–writers problem is sometimes proposed, which adds the constraint that no thread shall be allowed to starve; that is, the operation of obtaining a lock on the shared data will always terminate in a bounded amount of time. A solution with fairness for both readers and writers might be as follows:

```cpp
int readcount;                // init to 0; number of readers currently accessing resource

// all semaphores initialised to 1
semaphore resource;           // controls access (read/write) to the resource
semaphore rmutex;             // for syncing changes to shared variable readcount
semaphore serviceQueue;       // FAIRNESS: preserves ordering of requests (signaling must be FIFO)

//READER
reader() {
<ENTRY Section>
  serviceQueue.P();           // wait in line to be serviced
  rmutex.P();                 // request exclusive access to readcount
  readcount++;                // update count of active readers
  if (readcount == 1)         // if I am the first reader
    resource.P();             // request resource access for readers (writers blocked)
  serviceQueue.V();           // let next in line be serviced
  rmutex.V();                 // release access to readcount
    
<CRITICAL Section>
//reading is performed
    
<EXIT Section>
  rmutex.P();                 // request exclusive access to readcount
  readcount--;                // update count of active readers
  if (readcount == 0)         // if there are no readers left
    resource.V();             // release resource access for all
  rmutex.V();                 // release access to readcount
}

//WRITER
writer() {
<ENTRY Section>
  serviceQueue.P();           // wait in line to be serviced
  resource.P();               // request exclusive access to resource
  serviceQueue.V();           // let next in line be serviced
    
<CRITICAL Section>
// writing is performed
    
<EXIT Section>
  resource.V();               // release resource access for next reader/writer
}
```

### References
[Synchronization](https://en.wikipedia.org/wiki/Synchronization)<br>
[同步](https://zh.wikipedia.org/wiki/%E5%90%8C%E6%AD%A5)<br>
[Asynchronous I/O](https://en.wikipedia.org/wiki/Asynchronous_I/O)<br>
[Synchronization (computer science)](https://en.wikipedia.org/wiki/Synchronization_(computer_science))<br>
[Race condition](https://en.wikipedia.org/wiki/Race_condition#Software)<br>
[竞争冒险](https://zh.wikipedia.org/wiki/%E7%AB%B6%E7%88%AD%E5%8D%B1%E5%AE%B3)<br>
[Are "data races" and "race condition" actually the same thing in context of concurrent programming](https://stackoverflow.com/questions/11276259/are-data-races-and-race-condition-actually-the-same-thing-in-context-of-conc)<br>
[https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem](https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem)<br>
[读者写者问题（读者优先,写者优先 ,读写公平）](https://blog.csdn.net/william_munch/article/details/84256690)
