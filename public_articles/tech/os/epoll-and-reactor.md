本文尝试换一个角度来介绍epoll。想这么做的原因是因为，网上大部分文章对于epoll的使用细节，以及具体实现都有着非常详细的介绍，但是对于这个概念的引入，以及一些相关概念的边界，个人认为讲解的不是很清晰。所以，本文基于个人对于epoll的理解，给出一个初识版本的epoll

### I/O multiplexing

我们为什么需要I/O multiplexing，关于这个问题，我觉得csapp的回答是比较好的引入方式。

>Applications that use application-level concurrency are known as concurrent
programs. Modern operating systems provide three basic approaches for building
concurrent programs:

- Processes. With this approach, each logical control flow is a process that is
scheduled and maintained by the kernel. Since processes have separate virtual
address spaces, flows that want to communicate with each other must use some
kind of explicit interprocess communication (IPC) mechanism.
- I/O multiplexing.This is a form of concurrent programming where applications
explicitly schedule their own logical flows in the context of a single process.
Logical flows are modeled as state machines that the main program explicitly
transitions from state to state as a result of data arriving on file descriptors.
Since the program is a single process, all flows share the same address space.
- Threads. Threads are logical flows that run in the context of a single process
and are scheduled by the kernel. You can think of threads as a hybrid of the
other two approaches, scheduled by the kernel like process flows and sharing
the same virtual address space like I/O multiplexing flows.

csapp在chapter12(Concurrent Programming)当中引入了三种并发编程的方式，I/O multiplexing作为其中的一种被引入，我们首先来理解这个概念。

根据wikipedia给出的定义：
>In telecommunications and computer networks, multiplexing (sometimes contracted to muxing) is a method by which multiple analog or digital signals are combined into one signal over a shared medium. 

我们知道multiplexing这个概念，最早是在通信当中引入的，其意义表明多路信号通过一个共享介质，在形式上表现为单路信号的数据传输。其中，类比我们的并发编程语义，我们可以给出如下的对应关系
- multiple analog or signal -> multiple logical control flows
- one signal(are combined into) -> one process(one thread)
- a shared medium -> a shared address place

从而，我们很容易给出并发编程语义下，multiplexing的概念: **I/O multiplexing is a method by which multiple logical control flows are combined into one process over a shared address place**。此时，我们再对比csapp给出的定义，发现两种定义已经非常相似，但是后者当中有状态机概念的引入，此时我们需要引入下一个概念来辅助我们加深理解

### Event-driven programming

>In computer programming, event-driven programming is a programming paradigm in which the flow of the program is determined by events such as user actions (mouse clicks, key presses), sensor outputs, or message passing from other programs or threads. 
In an event-driven application, there is generally a main loop that listens for events and then triggers a callback function when one of those events is detected.

这里引入event-driven programming的原因在于，I/O multiplexing使用并发主要处理的是 **Accessing slow I/O devices** 这一类并发问题(多说一点，Computing in parallel on multi-core machines这一类并发问题，显然就不会用I/O multiplexing)，其处理对象就是各种i/o events.

具体会使用Event-driven finite-state machine这一种paradigm进行处理，伪代码如下
```c
/********************************************************************/
#include <stdio.h>

/********************************************************************/
typedef enum {
        ST_RADIO,
        ST_CD
} STATES;

typedef enum {
        EVT_MODE,
        EVT_NEXT
} EVENTS;

EVENTS readEventFromMessageQueue(void);

/********************************************************************/
int main(void)
{
  /* Default state is radio */  
  STATES state = ST_RADIO;
  int stationNumber = 0;
  int trackNumber = 0;

  /* Infinite loop */
  while (1)
  {
    /* Read the next incoming event. Usually this is a blocking function. */
    EVENTS event = readEventFromMessageQueue();

    /* Switch the state and the event to execute the right transition. */
    switch (state)
    {
      case ST_RADIO:
        switch (event)
        {
          case EVT_MODE:
            /* Change the state */
            state = ST_CD;
            break;
          case EVT_NEXT:
            /* Increase the station number */
            stationNumber++;
            break;
        }
        break;

      case ST_CD:
        switch (event)
        {
          case EVT_MODE:
            /* Change the state */
            state = ST_RADIO;
            break;
          case EVT_NEXT:
            /* Go to the next track */
            trackNumber++;
            break;
        }
        break;
    }
  }
}
```

根据fsm的定义
>In computation, a finite-state machine (FSM) is event driven if the transition from one state to another is triggered by an event or a message.

我们很容易得到如下的对应关系
previous state: state/stationNumber/trackNumber
event: EVENTS
driven(callback): event handler
after state: new state/stationNumber/trackNumber

我们将上述的事件映射为io events，则得到了I/O Multiplexing的处理情形，对应关系如下
- EVENTS -> read/write io event
- readEventFromMessageQueue -> select/poll/epoll->return fd which is ready(recv buffer is not empty/send buffer is not full)
- driven(callback) -> copy data from kernal space to user space or write data from user space to kernal space
- STATE -> application program state

此时我们再回头看I/O Multiplexing的定义，则很容易明白其意义：
- This is a form of concurrent programming where applications
explicitly schedule their own logical flows in the context of a single process.
- Logical flows are modeled as state machines that the main program explicitly
transitions from state to state as a result of data arriving on file descriptors.

第一句话表明，**I/O Multiplexing是一种处理并发的手段(特别注意其使用场景，io events)**。第二句话则说明其编程范式是事件驱动型，**具体通过fsm来实现**

### Epoll

有了以上的背景，此时再介绍epoll就非常简单了。一句话，I/O multiplexing的一种。可以理解为readEventFromMessageQueue的一种实现。其和select/poll的区别，我不再赘述，网上大量这种文章。
这里我想讲一个我之前对于EventLoop实现的误区，即对于io event ready理解的错误case

我们以select为例，下面是我自己封装的一个TcpServer,CalServer继承于TcpServer实现对应的HandleIOEvent函数，处理逻辑。具体就是拿到请求，计算两个数的和，写回

```cpp
void TcpServer::EventLoop() {
  // serving-loop
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_len = 0;

  fd_set reads, cpy_reads;
  FD_ZERO(&reads);
  FD_SET(listen_sfd_, &reads);

  int max_sfd = listen_sfd_;
  struct timeval timeout;

  while(1) {
    printf("Tcp server[localhost:%d] waiting...\n", ntohs(serv_addr_.sin_port));

    cpy_reads = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    int ret = select(max_sfd + 1, &cpy_reads, NULL, NULL, &timeout);
    if (ret == -1) {
      perr_handling("select", "error");
    } else if (ret == 0) {
      continue;
    } else {
      for (int sfd = 0; sfd <= max_sfd; ++sfd) {
        if (!FD_ISSET(sfd, &cpy_reads)) {
          continue;
        }
        // if: listen_sfd io event
        // else: other sfd io event
        if (sfd == listen_sfd_) {
          int clnt_sfd = accept(listen_sfd_, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
          if (clnt_sfd == -1) {
            perr_handling("accept", "error");
          }

          //Add clnt_sfd to select
          FD_SET(clnt_sfd, &reads);
          max_sfd = std::max(clnt_sfd, max_sfd);

          printf("[%s:%d] connected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        } else {
          HandleIoEvent(sfd);
          close(sfd);

          // Remove clnt_sfd to select
          FD_CLR(sfd, &reads);
          max_sfd = (max_sfd == sfd)?(max_sfd - 1):max_sfd;
          printf("[%s:%d] disconnected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        }
      }
    }
  }
}

void CalServer::HandleIoEvent(int clnt_sfd) {
  CalReq req;
  CalRes res;

  RecvReq(clnt_sfd, &req);

  DoCal(req, &res);

  SendRes(clnt_sfd, res);
}
```

上面代码属于典型的i/o multiplexing误用，具体分析如下
- event loop能成立的前提是，每一次event handle都一定是ready的，这样main loop不会被阻塞
- 上面的代码这个逻辑也成立，但是少监听了sfd
  - client_sfd ready，触发HandleIoEvent。但是后者不仅读(read is ready, no problem)，但是还会写
  - 如果此时kernal send buffer满，那么write操作会阻塞。event handler阻塞整个event-loop
- 上面代码真正的问题在于，read events handler实现的有误。正确的做法应该是三件事
  - 读数据(copy data from kernal recv buffer to application recv buffer)
  - 处理逻辑
  - 根据业务逻辑判断，如果有数据可写，更新writesfds
    - 将client_sfd加入writefds
    - 记录状态(下次写事件触发时，从哪写，写多少)
- 如此，上面正确做法的三件事，都不会阻塞main-loop

下面给一段正确的实现
```cpp
void copy(int from, int to) {
    char buffer[10];
    int readp = 0;
    int writep = 0;
    bool eof = false;
    for(;;) {
        fd_set readfds, writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);

        int ravail, wavail;
        if(readp < writep) {
            ravail = writep - readp - 1;
            wavail = sizeof buffer - writep;
        }
        else {
            ravail = sizeof buffer - readp;
            wavail = readp - writep;
        }

        if(!eof && ravail)
            FD_SET(from, &readfds);
        if(wavail)
            FD_SET(to, &writefds);
        else if(eof)
            break;
        int rc = select(max(from,to)+1, &readfds, &writefds, NULL, NULL);
        if(rc == -1)
            break;
        if(FD_ISSET(from, &readfds))
        {
            ssize_t nread = read(from, &buffer[readp], ravail);
            if(nread < 1)
                eof = true;
            readp = readp + nread;
        }
        if(FD_ISSET(to, &writefds))
        {
            ssize_t nwritten = write(to, &buffer[writep], wavail);
            if(nwritten < 1)
                break;
            writep = writep + nwritten;
        }
        if(readp == sizeof buffer && writep != 0)
            readp = 0;
        if(writep == sizeof buffer)
            writep = 0;
    }
}
```

### Reactor(Reactor Pattern)

紧接着我们来说reactor，首先给出wikipedia上的定义:
>The reactor design pattern is an event handling pattern for handling service requests delivered concurrently to a service handler by one or more inputs. The service handler then demultiplexes the incoming requests and dispatches them synchronously to the associated request handlers.

这句话给我们如下的信息
- 是什么：一种设计模式(对于常见问题的通用、可复用的解决办法)，基于事件驱动编程范式下的一种通用做法(经过精心设计的fsm)
- 具体使用场景：并发场景下，处理多组io events

根据其使用场景，很容易锁定I/O Multiplexing，同时，又是event handling pattern，很明显就是基于select/poll/epoll fsm精心设计的一种模式。实际当中是通过epoll来实现<br>
简而言之，Reactor是一种基于I/O Multiplexing和事件驱动的设计模式，主要用来(并发)处理多路低速IO设备

下面我们说一下实现，首先看一下结构
- Resources： Any resource that can provide input to or consume output from the system.
- Synchronous Event Demultiplexer： Uses an event loop to block on all resources. The demultiplexer sends the resource to the dispatcher when it is possible to start a synchronous operation on a resource without blocking (Example: a synchronous call to read() will block if there is no data to read. The demultiplexer uses select() on the resource, which blocks until the resource is available for reading. In this case, a synchronous call to read() won't block, and the demultiplexer can send the resource to the dispatcher.)
- Dispatcher: Handles registering and unregistering of request handlers. Dispatches resources from the demultiplexer to the associated request handler.
- Request Handler:An application defined request handler and its associated resource.

这里面有几个关键的点需要说下
- Resources/Request Handler:这两个很容易理解，就是event and event handler
- Synchronous Event Demultiplexer: 这个东西是什么？
  - 首先，需要特别关注的是，**Synchronous**，同步，同步，同步。重要的事说三遍。其实这里也有点故弄玄虚，这个东西就是fsm当中的readEventFromMessageQueue，只不过后者更通用。
  - 其次，虽然叫Demultiplexer，但是这个组件的功能是集Multiplexer/Demultiplexer于一身。多路fd通过epoll进行监听，最后有效的fd，也通过epoll返回。
  - 最后，为什么是同步的？因为这个函数是阻塞的，application code需要等这个函数返回。本质，epoll这个东西还是kernal通过阻塞的方式替application code把轮询这个事做了。设想，轮询的时候，fd不能是阻塞的吧。所以，epoll返回，fd确定可读/可写。对于application code，fd也需要non-blocking，因为fd返回时，可能只返回部分数据，比如返回了20 bytes，但是要读取30bytes，event handler此时会blocking event loop.
- Dispatcher: 这个负责注册fd，后者状态改变时，需要及时更新监听集合

下面说说这种设计模式的优点：
- Separation of concerns: The Reactor pattern decouples application-independent demultiplexing and dispatching mechanisms from application-specific hook method functionality. The application-independent mechanisms become reusable components that know how to demultiplex events and dispatch the appropriate hook methods defined by Event Handlers. In contrast, the application-specific functionality in a hook method knows how to perform a particular type of service.
- Improve modularity, reusability, and configurability of event-driven applications: The pattern decouples application functionality into separate classes. For instance, there are two separate classes in the logging server: one for establishing connections and another for receiving and processing logging records. This decoupling enables the reuse of the connection establishment class for different types of connection-oriented services (such as file transfer, remote login, and video-on-demand). Therefore, modifying or extending the functionality of the logging server only affects the implementation of the logging handler class.
- Improves application portability: The Initiation Dispatcher’s interface can be reused independently of the OS system calls that perform event demultiplexing. These system calls detect and report the occurrence of one or more events that may occur simultaneously on multiple sources of events. Common sources of events may in- clude I/O handles, timers, and synchronization objects. On UNIX platforms, the event demultiplexing system calls are called select and poll [1]. In the Win32 API [16], the WaitForMultipleObjects system call performs event demultiplexing.
- Provides coarse-grained concurrency control: The Reactor pattern serializes the invocation of event handlers at the level of event demultiplexing and dispatching within a process or thread. Serialization at the Initiation Dispatcher level often eliminates the need for more complicated synchronization or locking within an application process.

对比一下thread-per-connection的优缺点：
- Efficiency: Threading may lead to poor performance due to context switching, synchronization, and data movement [2];
- Programming simplicity: Threading may require complex concurrency control schemes;
- Portability: Threading is not available on all OS platforms

当然上面是论文中给出的观点，如果我们从csapp的建议来看，这么比相当于优点拿自己的长处和别人的短处比。因为并发处理有很多种，有处理低速设备，有处理并发计算的，业务情形不一样。select/poll/epoll就是为**Accessing slow I/O devices**这种业务而生的，那么thread-per-connection肯定不适用嘛!

关于reactor的几种实现，网上大量这种文章，最后一篇参考文献给出很好的解释。我在这里做简单解释

- Resources
- Synchronous Event Demultiplexer
- Dispatcher
- Request Handler
  - Acceptor
  - Handler

原有reactor的结构不变，Reactor具体的实现主要是对于Request Handler有了更细致的分工。有一类I/O事件是请求建立链接，其余一类是处理业务读/写的。对于这两类做了进一步的划分，分别具体了Acceptor/Handler来区别。
其中，多线程这里采用了池化资源的方式来避免频繁的线程开辟/回收。

关于这个几种变体的理解，我想举一个例子来类比，想想我们在一个盲人按摩中心。我们可以有如下类比：
- Acceptor -> 前台小姐
- Handler -> 按摩师傅

当有个人来时，首先需要前台小姐处理(记录必要信息)，其次将其派发给一个按摩师傅。前台小姐和按摩师傅的区别在于，前台小姐的工作比较简单，耗时低，按摩师傅的工作相对复杂，耗时高。所以，通常都是一个前台小姐，多个按摩师傅。根据这个场景，我们来理解reactor的各种变体。
- 单reactor/单线程：适用于业务处理快速的场景。如果某个handler耗时较高，会导致其他handler延迟高，毕竟只是一个event-loop.注意，这里不会阻塞，只是延迟高。缺点是多核性能无法利用，当然，优缺点的讨论一定要看场景。redis使用了这种方式
- 单reactor/多线程：这是最好理解的一种方式。优点是并发能力增强，多核处理能力利用。缺点是，引入多线程的缺点(共享资源/通用的竞争)。同时，Synchronous Event Demultiplexer(这个在这种场景下，也被叫做reactor)位于单线程中，激增大量请求时，处理可能成瓶颈。这么理解也有一些道理，此时reactor需要监听所有事件。
- 多reactor/多线程：这种就是把Synchronous Event Demultiplexer引入到子线程中。和第二种方式的对比是，前者Demultiplexer需要处理所有事件(请求IO事件以及IO业务读写事件都需要处理)。而后者主线程的Demultiplexer只负责监听请求到来的事件，而监听响应事件的能力，则下放到子线程中，由对应的sub reactor负责。Netty/Memcached都采用了这种方案

[Multiplexing](https://en.wikipedia.org/wiki/Multiplexing)<br>
[Event-driven_programming](https://en.wikipedia.org/wiki/Event-driven_programming)<br>
[Event-driven_finite-state_machine](https://en.wikipedia.org/wiki/Event-driven_finite-state_machine)<br>
[how-is-select-alerted-to-an-fd-becoming-ready](https://stackoverflow.com/questions/12625224/how-is-select-alerted-to-an-fd-becoming-ready)<br>
[Reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern)<br>
[Reactor模式详解](http://www.blogjava.net/dlevin/archive/2015/09/02/427045.html)<br>
[Reactor An Object Behavioral Pattern for Demultiplexing and Dispatching Handles for Synchronous Events](http://www.dre.vanderbilt.edu/~schmidt/PDF/reactor-siemens.pdf)<br>
[图解高性能网络架构：Reactor 和 Proactor](https://www.cnblogs.com/xiaolincoding/p/14706824.html)
