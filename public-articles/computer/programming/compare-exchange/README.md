```cpp
constexpr int kThres = 10;
std::atomic<int> counter{9};

std::mutex io_mtx;

void producer() {
  if (counter < kThres)
    ++counter;

  std::scoped_lock lock(io_mtx);
  std::cout << "thread:" << std::this_thread::get_id() << ", counter=" << counter << std::endl;
}

void test() {
  std::thread t1(producer);
  std::thread t2(producer);

  t1.join();
  t2.join();
}

int main(void) {
  test();
  return 0;
}

/* posible result:
thread:0x16f8bf000, counter=11
thread:0x16f94b000, counter=11

thread:0x16f84b000, counter=10
thread:0x16f7bf000, counter=10
*/
```

其可能得执行顺序如下：

1. Thread 1 reads counter value, which is 9， less than 10.
2. Thread 2 reads counter value, which is also 9, less than 10.
3. Thread 1 increments the counter to 10.
4. Thread 2 increments the counter to 11.

上面这段代码是非常经典的没有data race，但有race condition的情形，所以最终的结果不确定。

具体原因在于，对于counter的条件判断和自增，并不是原子的。可以通过加锁来解决。

### campare_exchange_xxx

```cpp
bool compare_exchange_weak(T& expected, 
                           T desired,
                           std::memory_order success,
                           std::memory_order failure ) noexcept;


bool compare_exchange_strong(T& expected, 
                             T desired,
                             std::memory_order success,
                             std::memory_order failure ) noexcept;
```

>Atomically compares the object representation (until C++20)value representation (since C++20) of *this with that of expected, 
and if those are bitwise-equal, replaces the former with desired (performs read-modify-write operation). Otherwise, loads the actual value stored in *this into expected (performs load operation).

```cpp
if (*this == expected) {
    *this = desired;
    return true;
}
else {
    expected = *this;
    return false;
}
```

正确的解法如下：

```cpp
void producer() {
  auto cur = counter.load(std::memory_order_relaxed);
  if (cur < kThres)
    counter.compare_exchange_strong(cur, cur + 1);

  std::scoped_lock lock(io_mtx);
  std::cout << "thread:" << std::this_thread::get_id() << ", counter=" << counter << std::endl;
}
/* final result:
thread:0x16fed3000, counter=10
thread:0x16fe47000, counter=10
*/
```

此时没有race condition出现，无论程序执行多少遍，结果都是10

其可能得执行顺序如下：

1. Thread 1 reads counter value, which is 9， less than 10.
2. Thread 2 reads counter value, which is also 9, less than 10.
3. Thread 1 **atomically** compares cur with counter, they are equal(both are 9), and replace counter with cur + 1, finally return true.
4. Thread 2 **atomically** compares cur with counter, they are not equal(cur is 9, but counter is 10), return false.

从中我们可以看出来，thread2在尝试更新的时候，先判断counter是否发生了变化，如果没有，即证明thread1没有先执行，那么它可以在此基础上自增。实际是，发现counter发生了变化，所以不能在当前值的基础上自增(即用cur + 1更新)。如果要自增，需要更新cur值才行。

所以，```compare_exchange```操作，有一个乍一看很奇怪的操作，即失败时，用atomic var的当前值，更新expected。我们可以理解为，如果判断失败了，用原子变量的最新值，更新了旧值。然后在最新的值的前提下再进行自增，此时就是没有问题的。


>The weak forms (1-2) of the functions are allowed to fail spuriously, that is, act as if *this != expected even if they are equal. When a compare-and-exchange is in a loop, the weak version will yield better performance on some platforms.

这也有个问题，既然相等，但是没有返回true。具体细节是什么？
1.更新原子变量，返回false
2.没更新原子变量，没更新expected，返回false
2.没更新原子变量，更新expected，返回false

### Ref

[C++ 中 std::atomic 类型的 compare_exchange 应该选择哪个版本？](https://www.zhihu.com/question/526769301/answer/2430798890)</br>
[Understanding std::atomic::compare_exchange_weak() in C++11](https://stackoverflow.com/questions/25199838/understanding-stdatomiccompare-exchange-weak-in-c11)</br>
[compare_exchange](https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange)