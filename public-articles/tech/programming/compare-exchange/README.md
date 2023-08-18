### Intro

先来看下面这一段程序

```cpp
#include <atomic>
#include <mutex>
#include <iostream>
#include <thread>

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

实际运行结果不确定(Apple clang version 14.0.0/Darwin Kernel Version 22.2.0)，代码中counter的值受```kThres```的限制，最大不会超过10，但实际结果中存在11的情形。

分析其可能的执行顺序如下：

1. Thread 1 reads counter value, which is 9， less than 10.
2. Thread 2 reads counter value, which is also 9, less than 10.
3. Thread 1 increments the counter to 10.
4. Thread 2 increments the counter to 11.

这段代码是非常经典的没有data race，但存在race condition的情形。结果不确定的原因在于**counter的条件判断和自增并不是原子的**。

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

其语义如下: (参见[compare_exchange](https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange))

>Atomically compares the object representation (until C++20)value representation (since C++20) of *this with that of expected, 
and if those are bitwise-equal, replaces the former with desired (performs read-modify-write operation). Otherwise, loads the actual value stored in *this into expected (performs load operation).

具体操作可用如下代码模拟
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
- 接口语义：判断this和expected是否相等，即compare，若相等，则更新为desired；否则，用this值更新expected.
- 接口返回值：更新成功，返回true；更新失败，返回false。更新失败这里，有一些细节，对于weak forms
  - this和expected确实不等，无法更新，反过来this更新expected，返回false
  - this和expected相等，但是不更新，返回false

下面展开说一下，首先对于```compare_exchange```操作，有一个乍一看很奇怪的操作，即失败时用atomic var的当前值更新expected。我们可以理解为如果判断不等，即用原子变量的最新值，更新了旧值。然后在最新的值的前提下再进行操作，此时就是没有问题的。因为desired值可能会依赖expected，expected更新到最新的值，desired才能是正确的值。

同时，这个expected，和desired，其本质是atomic var的旧值和新值。```compare_exchage```这么设计的原因在于，为了判断在并发环境下，atomic var的值是否更新了。如果和旧值一样，没有更新，继续操作。如果更新了，先需要对齐旧值，再进行更新。

一般的写法如下：
```
auto cur = atomic_var.load(std::memory_order_relaxed);
atomic_var.compare_exchange_xxx(cur, cur + x)
```

更新失败时会自动更新旧值。这么做的必要性是在并发环境下通过cas操作自动更新了，从而可以在新值的基础上再操作，不会有intro当中的race condition问题。

### Best Practice

最佳实践这里，主要是区分the strong forms and the weak forms的使用场景。我这里直接引[Understanding std::atomic::compare_exchange_weak() in C++11](https://stackoverflow.com/questions/25199838/understanding-stdatomiccompare-exchange-weak-in-c11)的内容，进行展开。

#### Typical Pattern A

You need achieve an atomic update based on the value in the atomic variable. A failure indicates that the variable is not updated with our desired value and we want to retry it. Note that we don't really care about whether it fails due to concurrent write or spurious failure. **But we do care that it is us that make this change.**

A real-world example is for several threads to add an element to a singly linked list concurrently. Each thread first loads the head pointer, allocates a new node and appends the head to this new node. Finally, it tries to swap the new node with the head.

说一下对这部分的理解，用法A的要求是：
- 线程判断原子变量的状态(compare)并确保完成更新(update)，一般适用于非bool变量。
- 相较于Typical Pattern B，后者只要求判断原子变量的状态，达到要求时，不再更新，一般适用于bool变量。

上文提到了一个最典型的例子就是官方文档当中的链表插入的例子，由于每个线程都需要保证自己的节点插入，但是因为头结点会不断更新。所以需要保证在拿到最新头结点的前提下，进行更新。所以，compare and update都需要执行。但是```compare_exchange```函数不一定可以成功，所以需要一个while-loop来确保最终一定可以更新成功。

代码写法如下
```cpp
auto cur = atomic_var.load(std::memory_order_relaxed);
while(!atomic_var.compare_exchange_xxx(cur, cur + x));
// the body of the loop is empty
```

这里我补充下，因为二刷的时候发现有点误会。这里得强调下，对于strong form它也是会失败的，只不过weak from失败的情形多了一种，即除了常规的失败，还有fail spuriously。所有，对于typical pattern，不管strong form还是weak form，都是while-loop的写法。此时，weak form可能性能会更好，下面是解释。

好，那么对于weak form/strong form，我们该选哪一种呢？

标准的建议如下:

>When a weak compare-and-exchange would require a loop and a strong one would not, the strong one is preferable unless the object representation of T may include padding bits, (until C++20) trap bits, or offers multiple object representations for the same value (e.g. floating-point NaN). In those cases, weak compare-and-exchange typically works because it quickly converges on some stable object representation.

这么选择的原因如下：

>The weak forms (1-2) of the functions are allowed to fail spuriously, that is, act as if *this != expected even if they are equal. When a compare-and-exchange is in a loop, the weak version will yield better performance on some platforms.

对于Intro的例子，如下是正确的实现

```cpp
#include <atomic>
#include <iostream>
#include <thread>

constexpr int kThres = 10;
std::atomic<int> counter{9};

std::mutex io_mtx;
void producer() {
  auto cur = counter.load(std::memory_order_relaxed);
  while (cur < kThres and not counter.compare_exchange_weak(cur, cur + 1));
}

void test() {
  std::thread t1(producer);
  std::thread t2(producer);

  t1.join();
  t2.join();

  std::cout << "counter=" << counter << std::endl;
}

int main(void) {
  test();
  return 0;
}
```

可以看到我们在while当中又加了一个判断，因为这里对原子变量的范围有限制。

#### Typical Pattern B

In contrary to pattern A, you want the atomic variable to be **updated once**, but you don't care who does it. As long as it's not updated, you try it again. This is typically used with boolean variables. E.g., you need implement a trigger for a state machine to move on. Which thread pulls the trigger is regardless.

说一下这部分的理解，用法B的要求是：
- 线程判断原子变量的状态(compare)并确保完成一次更新(update)，一般适用于bool变量。
- 相较于Typical Pattern A，后者并不要求当前线程需要完成更新，只要有线程完成更新即可。

很显然，如果用weak form需要一些多余的操作，主要体现在更新失败的时候，因为这有两种情形
- 该原子变脸已被其余线程更新，此时什么都不用做。
- fail spuriously，此时没有更新原子变量，继续更新。
- 整体写法还是需要while-loop，同时条件判断需要加上**对于当前原子变量是否更新**的判断，用来区别上面的两种情形。

所以，这种情形使用strong form会简单很多
```cpp
auto cur = atomic_var.load(std::memory_order_relaxed);
atomic_var.compare_exchange_strong(cur, cur + x);
```

这里还是二刷的补充，对比Typical Pattern A的写法，一目了然。因为此时不用每个线程都保证执行成功，所以不用while-loop.这里甚至都不用返回值，因为肯定有线程可以更新成功。即lock-free，程序不会全速前进，但至少有一个线程在前进。

我们甚至都不关心返回值，因为不管返回值如何，这个原子变量肯定更新了。 

进一步，我们再看一个[C++ 中 std::atomic 类型的 compare_exchange 应该选择哪个版本？](https://www.zhihu.com/question/526769301/answer/2430798890)当中的例子，bool型atomic，但是它有一个不同点，原子变量更新之后，需要再执行一个操作。即，哪一个线程完成了更新，哪一个线程来操作。

```cpp
State cur_state = State::Ready;
if (state_.compare_exchange_strong(cur_state, State::Closing)) {
  CloseAndCleanUp();
}
```

注意，这里是strong form没有什么好说的，不过它这里使用了返回值。返回值为true，代表是当前线程做的更新，那么它可以继续执行对应关闭状态的函数。如果返回false，证明其他线程已更新，已执行关闭函数，这里不用更新，也不能double CloseAndCleanUp.

如果我们强行要用weak form来写，代码如下

```cpp
State cur_state = State::Ready;
while (!state_.compare_exchange_weak(cur_state, State::Closing) and cur_state == State::Ready);
if (cur_state = State::Ready) {
  CloseAndCleanUp();
}
```

这个代码看起来就没有那么直观了
- 首先，while-loop，这是因为weak form因为fail spuriously的原因，必须搭配while-loop使用。否则，可能所有线程都更新不成功。
  - 但此时会导致一个问题，到底是哪个线程真正完成了更新呢，需要通过别的形式判断。
  - 如果旧值(cur_state)没有更新，就证明是fail spuriously,可以继续更新。
  - 如果旧值更新了，那证明，其余线程更新了，此时cur_state也会被原子变量重新加载，更新为当前值。跳出循环，不更新。
- 其次，如果当前线程更新，那么cur_stage不会被重新加载，可以保证是当前线程的更新，从而可以继续执行对应的函数。
- 否则，如果其余线程更新，当前线程一定会更新失败，此时cur_state会被重新加载到最新状态，if条件判断失败，表明不是当前线程更新，自然也不应该执行对应的函数。
- 最后，不能像strong form那样用返回值判断的原因在于，后者存在fail spuriously。只能通过cur_state是否更新判断。

最后的最后，一定要理解好```compare_exchange_xxx```的函数语义，以及其内部的操作。更新旧值，是一个看起来非常奇怪，但非常精髓的操作。

同时，记住如下结论
- Typical Pattern A使用weak form with while loop
- Typical Pattern B使用strong form without while loop
- weak form只能搭配while-loop使用(因为fail spuriously的原因)，strong from可以搭配while-loop使用，也可以不搭配。更进一步，weak form返回失败的情形，可能是真的不一致(atomic变量的值在上一次读取和当前发生了变化)，也有可能是假的不一致(即值是一致的，但是返回了false)，由于返回失败的情形我们不能判断真假，所以只能依赖它返回true的情形，这就是为什么它一定要搭配while-loop
- strong form也会更新失败，所以Typical Pattern A如果使用strong form，也需要搭配while-loop。只不过这种场景weak form的性能可能会更好，所以使用weak form with while loop.