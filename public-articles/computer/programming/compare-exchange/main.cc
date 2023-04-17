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

namespace cas1 {

void producer() {
  auto cur = counter.load(std::memory_order_relaxed);
  while (cur < kThres and not counter.compare_exchange_weak(cur, cur + 1));

  std::scoped_lock lock(io_mtx);
  std::cout << "thread:" << std::this_thread::get_id() << ", counter=" << counter << std::endl;
}

void test() {
  std::thread t1(producer);
  std::thread t2(producer);

  t1.join();
  t2.join();
}

std::atomic<bool> is_running{true};
void close() {}

void worker1() {
  auto cur_state = is_running.load(std::memory_order_relaxed);
  if (cur_state == true) {
    if (is_running.compare_exchange_strong(cur_state, false)) close();
  }
}

void worker11() {
  auto cur_state = is_running.load(std::memory_order_relaxed);
  if (is_running.compare_exchange_strong(cur_state, false)) close();
}

void worker2() {
  auto cur_state = is_running.load(std::memory_order_relaxed);
  while (cur_state == true and not is_running.compare_exchange_weak(cur_state, false));
  if (cur_state == true) close();
}

}  // namespace cas1

int main(void) {
  //non_cas::test();
  //cas::test();
  cas1::test();
  return 0;
}
