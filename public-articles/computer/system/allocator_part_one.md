### 前期杂记

#### memory alignment

[Purpose of memory alignment](https://stackoverflow.com/questions/381244/purpose-of-memory-alignment)

-  A memory address is said to be aligned when the data referenced by said address is b bits long, and said address is b-bits aligned.(首先，内存对齐是针对变量的地址而言)
- The CPU always reads at its word size (4 bytes on a 32-bit processor), so when you do an unaligned address access — on a processor that supports it — the processor is going to read multiple words.(这里说出了根本原因，增加了访存速度。因为cpu每次读取的单位都是word size，如果地址不按32-bits aligned，那么对于某一个变量的读取，需要多次读取操作)
- 4 bytes alignment means they are positioned in 4 bytes

#### Intro

- 首先说了什么是heap
- 其次，allocator管理heap
- 再者，说了allocator分两类
- 最后，说了怎么用standart allocator + application-specific allocator

#### 9.9.1 The malloc and free Functions

0000 0000 0000 0000  0  0x0000
...
...
0000 0000 0000 1111 15  0x00015

结合上面的图我们可以知道
- 左面的内存块，和右边的内存地址没有关系，就是个标识
- 至于这个标识，一次都读几个？cpu定，上面的图里是4个，所以内存对齐是，地址是4的倍数
- 但是csapp里面说是8，也行。看怎么读？
    - 如果是4个，每个是一个字节，相当于一次读4字节
    - 如果是8个，每个是一个字节，相当于一次读8字节

然后讲了一个例子，cpu如果每次读双字，一个字4-bytes，那么内存对齐也必须按照这个来。
所以，分配的时候，考虑到内存对齐，就必须padding.
比如，2 words是cpu读取的长度，那么malloc每次至少是2 words，如果少于这个值，padding.

二刷，上面说的不太对。有关连，内存按字节组织

0000 0000 byte 0 (0x00000000)
0000 0000 byte 1 (0x00000001)
...
0000 0000 byte 15 (0x000000015)

左边是按纯二进制来组织内存数据，右边是地址。cpu32位，那么地址一定32位。
发现一个规律，带十六进制表示的，都是内存地址。

#### 9.9.2 Why Dynamic Memory Allocation?

The most important reason that programs use dynamic memory allocation is that
often they do not know the sizes of certain data structures until the program
actually runs.

#### 9.9.3 Allocator Requirements and Goals

- Ruirements
    - Handling arbitrary request sequences.(malloc/free的顺序不能保证，就是随机的来，不能假设其中的顺序，这隐式的显示了我们的分配策略。比如，如果刚来都是malloc，那么我可以一次分配多点。下次来啦，就移动指针即可，但实际不行)
    - Making immediate responses to requests.(限制了分配策略，no buffer, no reorder)
    - Using only the heap.(这话我觉得是废话，但我也没看懂他的解释)
    - Aligning blocks(对齐这个不用说了，为了cpu)
    - Not modifying allocated blocks.(no packation)

- Goals
    - 1.快。处理请求快
    - 2.高。内存使用率高

#### 9.9.4 Fragmentation

- 碎片
    - 内部。padding导致。
    - 外部。总和大小够，但是也不能分配。
    - 少量大块，好于大量小块(没懂)

#### 9.9.5 Implementation Issues

这一小节先说了非常理性的情形，那我们就顺序管理，来一个请求，移动heap pointer即可。但是问题很明显，不能复用。并且如果请求多，很快就用完了。

A practical allocator that strikes a better balance between throughput and utilization must consider the following issues:

- Free block organization. How do we keep track of free blocks?(这个我理解非常重要，因为每次分配，其实就是从free block进行分配，所以free block的管理尤为重要)
- Placement. How do we choose an appropriate free block in which to place a
newly allocated block?(这个和上面的配合，讨论的都是如何分配的问题，怎么样选一个块出来，会直接影响utilization)
- Splitting. After we place a newly allocated block in some free block, what do
we do with the remainder of the free block?(紧接着上面的问题，怎么处理伤口，晾着？缝合？具体到技术，头尾相连，还是分开，或者compaction再调整？)
- Coalescing. What do we do with a block that has just been freed?(对于被释放的块，怎么合入？)

其实allocated block，归app管。allocator主要就是考虑free block