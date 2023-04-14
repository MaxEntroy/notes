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
  std::thread t3(producer);

  t1.join();
  t2.join();
  t3.join();
}

int main(void) {
  test();
  return 0;
}
