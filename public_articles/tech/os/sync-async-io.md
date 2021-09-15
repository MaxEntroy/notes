### 同步/异步/阻塞/非阻塞

这几个概念容易搞混，主要是因为没有限定好讨论问题的背景，所以当我们特别讨论这几个概念时，一定是在如下的背景
- Socket programming
- I/O model
- Function call

#### 同步/异步

microsoft从file I/O的角度给出了定义
>In synchronous file I/O, a thread starts an I/O operation and immediately enters a wait state until the I/O request has completed. A thread performing asynchronous file I/O sends an I/O request to the kernel by calling an appropriate function. If the request is accepted by the kernel, the calling thread continues processing another job until the kernel signals to the thread that the I/O operation is complete. It then interrupts its current job and processes the data from the I/O operation as necessary.

从时空角度来看
- 同步IO(Sequential IO)
  - 时间：线性
  - 空间：连续
- 异步IO(Non-sequential IO)
  - 时间：非线性
  - 空间：非连续

这里面时空是否线性，连续，时区分二者的根本原因。

  <img width="500"  src="img/sync-async-io.png"/>