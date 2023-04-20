### Intro

先来看下面这一段程序

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

我们发现其运行结果不确定(Apple clang version 14.0.0/Darwin Kernel Version 22.2.0)，代码中counter的值受```kThres```的限制，最大不会超过10，但实际结果中存在11的情形。

分析其可能的执行顺序如下：

1. Thread 1 reads counter value, which is 9， less than 10.
2. Thread 2 reads counter value, which is also 9, less than 10.
3. Thread 1 increments the counter to 10.
4. Thread 2 increments the counter to 11.

这段代码是非常经典的没有data race，但有race condition的情形。最终的结果不确定原因在于**counter的条件判断和自增，并不是原子的**。

常规的办法我们可以通过加锁来解决，但atomic为我们提供了更高效的武器，即```campare_exchange_xxx```操作来实现(本质是CAS).

### General form

API如下

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

其语义可用如下的代码模拟：

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

上面这一组接口，有以下几点需要注意：
- 接口语义：是判断this和expected是否相等，即compare，若相等，则更新为desired；否则，用this值更新expected.
- 接口返回值：更新成功，返回true；更新失败，返回false。更新失败这里，有一些细节，对于weak forms
  - this和expected确实不等，无法更新，反过来this更新expected，返回false
  - this和expected相等，但是不更新，返回false

下面展开说一下，首先对于```compare_exchange```操作，有一个乍一看很奇怪的操作，即失败时，用atomic var的当前值，更新expected。我们可以理解为，如果判断不等，即用原子变量的最新值，更新了旧值。然后在最新的值的前提下再进行操作，此时就是没有问题的。因为desired值可能会依赖expected，expected更新到最新的值，desired才能是正确的值。

同时，这个expected，和desired，其本质是atomic var的旧值和新值。```compare_exchage```这么设计的原因在于，就是为了判断在并发环境下，atomic var的值是否更新了。如果和旧值一样，没有更新，继续操作。如果更新了，先需要对齐旧值，再进行更新。

一般的写法如下：
```
auto cur = atomic_var.load(std::memory_order_relaxed);
atomic_var.compare_exchange_xxx(cur, cur + x)
```

更新失败时，会自动更新旧值。这样的好处是，在并发环境下，通过cas操作自动更新了，从而可以在新值的基础上，再操作。

>The weak forms (1-2) of the functions are allowed to fail spuriously, that is, act as if *this != expected even if they are equal. When a compare-and-exchange is in a loop, the weak version will yield better performance on some platforms.

这也有个问题，既然相等，但是没有返回true。具体细节是什么？
1.更新原子变量，返回false
2.没更新原子变量，没更新expected，返回false
2.没更新原子变量，更新expected，返回false

### Ref

[C++ 中 std::atomic 类型的 compare_exchange 应该选择哪个版本？](https://www.zhihu.com/question/526769301/answer/2430798890)</br>
[Understanding std::atomic::compare_exchange_weak() in C++11](https://stackoverflow.com/questions/25199838/understanding-stdatomiccompare-exchange-weak-in-c11)</br>
[compare_exchange](https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange)