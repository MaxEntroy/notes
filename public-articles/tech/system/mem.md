## Cache

### False sharing

Eeach thread uses its own array element, also has no shared data. However, it performs as if the data was shared between threads, at least for a small number of threads, so we call it False sharing: nothing is really shared, but the program behaves as if it was.

这两有两点注意：
- 其一，false sharing主要是从性能的维度来描述现象，没有共享，但是性能表现得像共享一样差。
- 其二，不共享这个表达没有问题。对于local copy而言，也有可能位于同一个cacheline上，但是概率非常低。
  - 但是，对于数组而言，相邻元素位于同一个cache line概率非常高。
  - 同时，虽然数组元素不共享，即不同线程并不操作数组的同一个元素。但是，整个数组是共享的。即不同的线程还是要读取同一份数组到cache line当中。
  - 如果只是thread local copy，这是彻底的不共享。
  - 对于此，最佳实践是：对于共享数组，但是不操作相同内容的诉求来说。没有data race，但是可能存在false sharing. 解决办法是，让数组元素和cache line对其，从而他们能位于不同的cache line当中，避免false sharing.

  好，下面讨论一下，为什么位于同一个cache line当中，性能会非常差呢？

### Cache line

- The CPU has direct access only to the L1 cache, so it gets the datafrom there. How does the data get from the main memory into the cache? It's copied overthe memory bus, which is much wider. 
- The minimum amount of data that can be copied from memory to cache and back is called a cache line.

- Now we can explain the false sharing we observed: even though the adjacent array elements are not really shared between threads, they do occupy the same cache line.
- When a CPU requests exclusive access to one array element for the duration of the atomic increment operation, it locks the entire cache line and prevents any other CPU from accessing any data in it.

That exclusive cache line access is the origin of the high cost of shared variables.

根本原因在于cpu对于cache line的独占访问，如果多核共享了同一个cache line副本，当其中的一个cpu对cache line进行变更时，独占访问，其余核此时不能更新当前cacheline数据。从而造成了其实没有共享，但是实际共享的问题。

### Cache line bouncing

以上的false sharing同时也会引起另一个问题，就是cache line bouncing.

Coherent L1 caches are implemented with some version or variant of the MESI protocol. In this protocol, a given core has to "own" a cache line before being able to write to it. If the cache line is already owned by another core, there is a mechanism to request that ownership be transferred and the updated data copied over, but this takes time.

上面说到，core对于cache line是独占访问的。这个是怎么保证的呢？这里回到了，通过MESI协议。申请独占访问时，需要request transfer。这个行为会促使cache line中的数据更新。

If two cores are frequently writing to the same variable (or to two different variables in the same cache line, in the case of false sharing), then most of the writes will need such a transfer, slowing everything down. But if only one core needs to write it, the transfer is not required, so it is faster.

这一段说明了，如果多核频繁的访问同一个cache line，那么上述的ownership transfer会频繁发生，后者的开销大。

简单总结下：
- cache line的访问需要独占
- 独占访问由MESI机制保证，ownership transfer会发生，伴随着cache line更新，存在开销。
- 多核频繁访问cache line会导致频繁的ownership transfer，开销陡增。
- 看起来像是cache line在多核之间bouncing.

### Ref

[how-does-bouncing-variables-between-two-local-l1-caches-makes-the-code-slower](https://stackoverflow.com/questions/72831563/how-does-bouncing-variables-between-two-local-l1-caches-makes-the-code-slower)<br>