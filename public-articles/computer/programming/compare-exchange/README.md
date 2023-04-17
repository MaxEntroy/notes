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

Atomically compares the object representation (until C++20)value representation (since C++20) of *this with that of expected, 
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