#include <atomic>
#include <mutex>
#include <iostream>
#include <thread>

constexpr int kThres = 10;
std::atomic<int> counter{9};

std::mutex io_mtx;

namespace non_cas {

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

}  // namespace non-cas

namespace cas {

void producer() {
  auto cur = counter.load(std::memory_order_relaxed);
  if (cur < kThres)
    counter.compare_exchange_strong(cur, cur + 1);

  std::scoped_lock lock(io_mtx);
  std::cout << "thread:" << std::this_thread::get_id() << ", counter=" << counter << std::endl;
}

void test() {
  std::thread t1(producer);
  std::thread t2(producer);

  t1.join();
  t2.join();
}

}  // namespace cas

int main(void) {
  //non_cas::test();
  cas::test();
  return 0;
}
